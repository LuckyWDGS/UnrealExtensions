﻿// Fill out your copyright notice in the Description page of Project Settings.
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
	/* 写入文本
	* @param	SaveString				要保存的文字内容
	* @param	FileName				写入到指定的文件
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealExtensions|Data")
		static bool WriteTXT(const FString SaveString, const FString FileName);

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

	/* Comand打开文件
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

	/* 获取关卡的游戏路径  /Game/../../
	* @param	WorldContextObject	世界上下文对象
	*/
	UFUNCTION(BlueprintPure, Category = "UnrealExtensions|Paths", meta = (WorldContext = "WorldContextObject"))
		static  FString GetLevelPath(const UObject* WorldContextObject);

	/*
	* 加载图像来自硬盘
	* @param	ImagePath	图像路径
	* @param	IsValid		是否有效
	* @param	OutWidth	返回图像的宽
	* @param	OutHeight	返回图像的高
	*/
	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "2"),Category = "UnrealExtensions|Texture")
		static UTexture2D* LoadTexture2D(const FString& ImagePath, bool& IsValid, int32& OutWidth, int32& OutHeight);

	/*
		* 转换并调整图像大小
		* @param	ImagePath			要处理的图片路径
		* @param	SaveFilePath		要保存的图片路径包含扩展名.jpg
		* @param	NewWidth			新的分辨率宽的大小 
		* @param	NewHeight			新的分辨率高的大小
		* @param	Quality				质量默认为75
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealExtensions|Image Conversion")
		static bool ConvertAndResizeImage(const FString& ImagePath, const FString& SaveFilePath,  int32 NewWidth, int32 NewHeight, int32 Quality = 75);

	/* 从AssetData中获取对象
	*  @param		AssetData			资产数据
	*/
	UFUNCTION(BlueprintPure, Category = "UnrealExtensions|StaticLoad")
		static UClass* StaticLoadClass(const FString& Reference);

	/* 从AssetData中获取对象
	*  @param		AssetData			资产数据
	*/
	UFUNCTION(BlueprintPure, Category = "UnrealExtensions|StaticLoad")
		static  UObject* LoadObjectFormAssetData(const FAssetData& AssetData);

	/* 从路径获取静态Object对象/Game/../..
	*  @param		FString			要加载的对象路径
	*/
	UFUNCTION(BlueprintPure, Category = "UnrealExtensions|StaticLoad")
		static  UObject* LoadObject(const FString& Reference);

	/* 获取类的默认Object对象/Game/../..
	 *  @param		Class			类
	 */
	UFUNCTION(BlueprintPure, Category = "UnrealExtensions|StaticLoad")
		static  UObject* GetClassDefaultObject(TSubclassOf<UObject>Class);

	/* 场景截图并且保存到目录
	*  @param		CaptureComponent2D		需要指定一个UE 2D相机
	*  @param		SavePath				截图需要保存的路径
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealExtensions|ScreenShot")
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
		UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "1"), Category = "UnrealExtensions|Sound")
			static class USoundWave* SoundFormByteData(TArray<uint8> RawWaveData,FString SavePath,bool IsSave=false);

	/* 音频文件转换成Sound对象
	*  @param		FileName				文件路径
	*  @param		return					返回一个USoundWave对象
	*/
		UFUNCTION(BlueprintCallable, Category = "UnrealExtensions|Sound")
			static class USoundWave* SoundFormFile(FString FileName);


/******************************************************************UMG类工具函数***********************************************************************/

/******************************************************************Calculate函数***********************************************************************/
	
	/* 计算FPS计时
	*/
		UFUNCTION(BlueprintPure, Category = "UnrealExtensions|Calculate",meta = (WorldContext = "WorldContextObject"))
			static int CalculateFPSTimings(const UObject* WorldContextObject);


		UFUNCTION(BlueprintCallable, Category = "UnrealExtensions|Call Func")
			static bool CallFuncByName(UObject* TargetObject, FName FunctionName);

		UFUNCTION(BlueprintCallable, Category = "UnrealExtensions|Call Func")
			static bool CallFuncByName_String(UObject* TargetObject, FName FunctionName, FString Param);

private:
};

