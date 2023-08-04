// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UnrealExtensionsType.h"
#include "UnrealExtensionsBPFLibrary.generated.h"


UCLASS()
class UNREALEXTENSIONS_API UUnrealExtensionsBPFLibrary : public UBlueprintFunctionLibrary 
{
	GENERATED_BODY()

public: 
	/* 获取当前的场景设置
	 */

	UFUNCTION(BlueprintPure, Category = "UnrealExtensions|WorldSettings", meta = (WorldContext = "WorldContextObject"))
		static  AWorldSettings* GetWorldSetting(const UObject* WorldContextObject);

	/* 获取文本内容
	* @param	TXTPath				文件路径或者文件夹路径
	* @param	OutTXT				输出的文本
	*/
	UFUNCTION(BlueprintPure, Category = "UnrealExtensions|Data")
		static TArray<FString> LoadTXT(const FString TXTPath, FString& OutTXT);

   /* 获取文件或文件夹数据，包括文件创建时间大小等信息
	* @param	InPath				文件路径或者文件夹路径
	* @param	FileData			文件数据
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealExtensions|Data")
		static void GetStatData(const FString InPath, FFileData& FileData);

	/* 是否为编辑器模式
	 */
	UFUNCTION(BlueprintPure, Category = "UnrealExtensions|Operate")
		static bool IsWithEditor();

	/* 打开文件夹
	 * @param	InPath	要打开的文件夹路径
	 */
	UFUNCTION(BlueprintCallable, Category = "UnrealExtensions|Operate")
		static bool ExploreFolder(const FString InPath);

	/* 获取路径下的所有文件夹，不存在递归
	 * @param	InPath			选择要查找的路径
	 * @param	OutPutFolder	输出文件夹路径
	 */
	UFUNCTION(BlueprintCallable, Category = "UnrealExtensions|Operate")
		static bool FindDirectory(const FString InPath,TArray<FString>&OutPutFolder,bool RemovePath=false);

	/* 打开文件
	 * @param	InPath			需要打开的文件路径
	 */
	UFUNCTION(BlueprintCallable, Category = "UnrealExtensions|Operate")
		static bool OpenbyFile(const FString InPath);

	/*
	* 正则匹配 如果匹配成功，则返回匹配到的字符串，否则返回false
	* @param	Str			输入要匹配的字符串
	* @param	Pattern		正则表达式
	* @param	Result		返回匹配到的字符串
	*/
	UFUNCTION(BlueprintPure, Category = "UnrealExtensions|Regex")
		static bool RegexMatch(const FString& Str, const FString& Pattern, TArray<FString>& Result);

	///获取关卡的游戏路径  /Game/../../
	UFUNCTION(BlueprintPure, Category = "UnrealExtensions|Paths", meta = (WorldContext = "WorldContextObject"))
		static  FString GetLevelPath(const UObject* WorldContextObject);

	//载入图像
	UFUNCTION(BlueprintPure,Category = "ExtendedContent|File")
		static UTexture2D* LoadTexture2D(const FString& ImagePath, bool& IsValid, int32& OutWidth, int32& OutHeight);

	/*
		* 正则匹配 如果匹配成功，则返回匹配到的字符串，否则返回false
		* @param	ImagePath			要处理的图片路径
		* @param	SaveFilePath		要保存的图片路径包含扩展名.jpg
		* @param	NewWidth			新的分辨率宽的大小 
		* @param	NewHeight			新的分辨率高的大小
		* @param	Quality				质量默认为75
	*/
	UFUNCTION(BlueprintCallable, Category = "ExtendedContent|Image Conversion")
		static bool ConvertAndResizeImage(const FString& ImagePath, const FString& SaveFilePath,  int32 NewWidth, int32 NewHeight, int32 Quality = 75);

	/* 从资产中获取当前蓝图的对象
	*  @param		AssetData			资产数据
	*/
	UFUNCTION(BlueprintPure, Category = "UnrealExtensions|UObject")
		static  UObject* GetBlueprintFormAssetData(const FAssetData& AssetData);

	/* 场景截图并且保存到目录
	*  @param		CaptureComponent2D		需要指定一个UE 2D相机
	*  @param		SavePath				截图需要保存的路径
	*/
	UFUNCTION(BlueprintCallable, Category = "ExtendedContent|ScreenShot")
		static void ScreenShot(class USceneCaptureComponent2D* CaptureComponent2D,const FString& SavePath);
		//将颜色数据提取
		static void ColorToImage(const FString& InImagePath, TArray<FColor>InColor, int32 InWidth, int32 InHight);//将颜色数据提取

/******************************************************************音频类函数***********************************************************************/
	/* 字节数组转换成音频Sound对象
	*  @param		RawWaveData				音频数据流
	*  @param		SavePath				音频需要保存的路径，需在IsSave为true时生效
	*  @param		IsSave					音频是否需要保存到本地
	*  @param		return					返回一个USoundWave对象
	*/
		UFUNCTION(BlueprintCallable, Category = "ExtendedContent|音频",meta = (AdvancedDisplay = "1", UnsafeDuringActorConstruction = "true"))
			static class USoundWave* SoundForByteData(TArray<uint8> RawWaveData,FString SavePath,bool IsSave=false);

private:
	
};

