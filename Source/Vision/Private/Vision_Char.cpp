// Developer: Lucas Vilas-Boas
// Year: 2022

#include "Vision_Char.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "ImageWriteBlueprintLibrary.h"

#include "HttpModule.h"

AVision_Char::AVision_Char()
{
 	PrimaryActorTick.bCanEverTick = false;

	RootComponent = GetCapsuleComponent();
	GetCapsuleComponent()->InitCapsuleSize(42.7f, 92.5f);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	FollowCamera->SetupAttachment(RootComponent);
	FollowCamera->SetRelativeLocation(FVector(0.f, 0.f, 92.5f));
	FollowCamera->bUsePawnControlRotation = true;

	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
	SceneCapture->SetupAttachment(FollowCamera);
	SceneCapture->bCaptureEveryFrame = false;
	SceneCapture->bCaptureOnMovement = false;
	SceneCapture->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_RenderScenePrimitives;
	SceneCapture->CaptureSource = ESceneCaptureSource::SCS_FinalToneCurveHDR;
	
	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> 
		RenderObj(TEXT("TextureRenderTarget2D'/Game/Main/Textures/RTX_Main_VisionTarget.RTX_Main_VisionTarget'"));
	if (RenderObj.Object != nullptr)
	{
		SceneCapture->TextureTarget = RenderObj.Object;
	}
}

void AVision_Char::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AVision_Char::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AVision_Char::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("MousePitch"), this, &AVision_Char::MousePitch);
	PlayerInputComponent->BindAxis(TEXT("MouseYaw"), this, &AVision_Char::MouseYaw);
}

void AVision_Char::BeginPlay()
{
	Super::BeginPlay();

	FString JsonStr;
	FFileHelper::LoadFileToString(JsonStr, *JsonPath);
	
	const TSharedRef<TJsonReader<>> MyJsonReader = TJsonReaderFactory<>::Create(JsonStr);
	FJsonSerializer::Deserialize(MyJsonReader, MyJsonObject);
}

void AVision_Char::MoveForward(const float Value)
{
	if (Value == 0.f) return;
	const FRotator YawRotation(0, GetControlRotation().Yaw, 0);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AVision_Char::MoveRight(const float Value)
{
	if (Value == 0.f) return;
	const FRotator YawRotation(0, GetControlRotation().Yaw, 0);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void AVision_Char::MousePitch(const float Value)
{
	if (Value == 0.f) return;
	AddControllerPitchInput(Value * 45.f * GetWorld()->GetDeltaSeconds());
}

void AVision_Char::MouseYaw(const float Value)
{
	if (Value == 0.f) return;
	AddControllerYawInput(1.f * Value * 45.f * GetWorld()->GetDeltaSeconds());
}

void AVision_Char::SaveSceneRenderToFile(const FString ImagePath)
{
	SceneCapture->CaptureScene();

	const FString SecurePath = ImagePath.EndsWith("\\") ? ImagePath : ImagePath + "\\";
	const FString QualifiedPath = SecurePath + "RenderedScene" + ".jpg";

	FImageWriteOptions IW_Options;
	IW_Options.bAsync = true;
	IW_Options.bOverwriteFile = true;
	IW_Options.CompressionQuality = 0;
	IW_Options.Format = EDesiredImageFormat::JPG;

	TFunction<void (bool)> CompletionResult = [QualifiedPath](const bool Result) -> bool
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, 
			FString::Printf(TEXT("Saving render to disk: Result: %u; Path: %s"), Result, *QualifiedPath));
		
		UE_LOG(LogTemp, Warning, TEXT("Saving render to disk: Result: %u; Path: %s"), Result, *QualifiedPath);

		return Result;
	};

	IW_Options.NativeOnComplete = CompletionResult;

	UImageWriteBlueprintLibrary::ExportToDisk(SceneCapture->TextureTarget.Get(), QualifiedPath, IW_Options);
}

void AVision_Char::Recognize_ComputerVision(const FString FileName)
{
	const FString Endpoint = MyJsonObject->GetStringField("ComputerVision_Endpoint");
	const FString SubscriptionKey = MyJsonObject->GetStringField("ComputerVision_SubscriptionKey");

	const FString Parameters = "visualFeatures=Tags";
	const FString Uri = Endpoint + "/vision/v2.0/analyze?" + Parameters;

	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader("Ocp-Apim-Subscription-Key", SubscriptionKey);
	HttpRequest->SetVerb("POST");
	HttpRequest->SetURL(Uri);
	HttpRequest->SetContentAsStreamedFile(FileName);
	HttpRequest->SetHeader("Content-Type", "application/octet-stream");

	HttpRequest->OnProcessRequestComplete().BindLambda([](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) -> void
		{
			const FString ResponseStr = Response->GetContentAsString();
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, ResponseStr);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *ResponseStr);
		});

	HttpRequest->ProcessRequest();
}

void AVision_Char::Recognize_CustomVision(const FString FileName)
{
	const FString Endpoint = MyJsonObject->GetStringField("CustomVision_Endpoint");
	const FString SubscriptionKey = MyJsonObject->GetStringField("CustomVision_SubscriptionKey");
	const FString PredictionKey = MyJsonObject->GetStringField("CustomVision_PredictionKey");
	const FString ProjectId = MyJsonObject->GetStringField("CustomVision_ProjectId");
	const FString ModelName = MyJsonObject->GetStringField("CustomVision_ModelName");

	const FString Uri = Endpoint +
		"/customvision/v3.0/Prediction/" + ProjectId + "/classify/iterations/" + ModelName + "/image";

	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader("Prediction-key", PredictionKey);
	HttpRequest->SetHeader("Training-key", SubscriptionKey);
	HttpRequest->SetHeader("Content-Type", "application/octet-stream");
	HttpRequest->SetVerb("POST");
	HttpRequest->SetURL(Uri);
	HttpRequest->SetContentAsStreamedFile(FileName);

	HttpRequest->OnProcessRequestComplete().BindLambda([](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) -> void
		{
			const FString ResponseStr = Response->GetContentAsString();
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, ResponseStr);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *ResponseStr);
		});

	HttpRequest->ProcessRequest();
}



