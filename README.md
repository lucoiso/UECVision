# UECVision

[Unreal Engine Forum](https://forums.unrealengine.com/t/computer-vision-inside-the-engine-microsoft-azure-custom-vision-and-computer-vision/507175?u=lucoiso)  
**This project was built on Unreal Engine 5.**

![HighresScreenshot00000](https://user-images.githubusercontent.com/77353979/158843292-6282029b-fd67-46a8-8b3b-d093b3ad4939.png)

## About

This project integrates Microsoft Azure Custom Vision and Microsoft Azure Computer Vision into Unreal Engine via C++ REST APIs.

![image](https://user-images.githubusercontent.com/77353979/158843490-7c671ab0-3e20-4bbc-b3e5-c23eaa095a9e.png)

## How To

1. Set up your Azure Resources:  
1.1. Custom Vision: [Custom Vision Resource](https://azure.microsoft.com/en-us/services/cognitive-services/custom-vision-service/) and [Custom Vision Portal](https://www.customvision.ai/);  
1.2. Computer Vision: [Computer Vision Resource](https://azure.microsoft.com/en-us/services/cognitive-services/computer-vision/);

2. Fill your .json file with services' informations. There's a template with the needed informations: [azureinfo_template.json](https://github.com/lucoiso/UECVision/blob/main/azureinfo_template.json);  

![image](https://user-images.githubusercontent.com/77353979/158842656-b1de6ce0-f11b-445d-8cc9-8b9662ee6176.png)

3. Set the path to save your rendered scene and the filename to send via REST apis in BP_Character_Main class. _If you want to use the rendered scene, just use Path + RenderedScene.jpg._  

![image](https://user-images.githubusercontent.com/77353979/158842200-cb3326a3-4b9f-4f73-970a-5fa660b9ba81.png)

4. Set your json file location in BP_Character_Main class.  

![image](https://user-images.githubusercontent.com/77353979/158842233-4df8f698-8c80-4b94-8c9a-8fce0e34b6b5.png)

