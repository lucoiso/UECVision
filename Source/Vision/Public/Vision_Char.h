// Developer: Lucas Vilas-Boas
// Year: 2022

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Vision_Char.generated.h"

UCLASS()
class VISION_API AVision_Char : public ACharacter
{
	GENERATED_BODY()

public:
	AVision_Char();

protected:
	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* FollowCamera;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

	void MoveForward(const float Value);
	void MoveRight(const float Value);
	void MousePitch(const float Value);
	void MouseYaw(const float Value);

	UPROPERTY(EditAnywhere, Category = "Custom Properties")
	class USceneCaptureComponent2D* SceneCapture;

	UPROPERTY(EditAnywhere, Category = "Custom Properties")
	FString JsonPath;

	TSharedPtr<FJsonObject> MyJsonObject = MakeShareable(new FJsonObject());

public:
	UFUNCTION(BlueprintCallable, Category = "Custom Functions")
	void SaveSceneRenderToFile(const FString ImagePath);

	UFUNCTION(BlueprintCallable, Category = "Custom Functions")
	void Recognize_ComputerVision(const FString FileName);

	UFUNCTION(BlueprintCallable, Category = "Custom Functions")
	void Recognize_CustomVision(const FString FileName);
};
