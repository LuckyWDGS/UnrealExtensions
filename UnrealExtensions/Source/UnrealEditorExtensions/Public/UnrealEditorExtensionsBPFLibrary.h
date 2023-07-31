// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UnrealEditorExtensionsType.h"
#include <ToolMenus/Public/ToolMenu.h>
#include "UnrealEditorExtensionsBPFLibrary.generated.h"


UCLASS()
class UNREALEDITOREXTENSIONS_API UUnrealEditorExtensionsBPFLibrary : public UBlueprintFunctionLibrary 
{
	GENERATED_BODY()

public: 

	//获取打开的对象
	UFUNCTION(BlueprintCallable, Category = "UnrealEditorExtensions|Path")
		static UObject* GetOpenObjecFromToolMenuContext(const FToolMenuContext& InContext);

	//获取内容浏览器选中的文件夹
	UFUNCTION(BlueprintPure, Category = "UnrealEditorExtensions|ContentBrowser")
		static TArray<FString> GetSelectedFolders();
	//获取所有的工具菜单栏
	UFUNCTION(BlueprintCallable, Category = "UnrealEditorExtensions|Tool Menus")
		static TMap<FName, UToolMenu*> GetAllMenu();
	/*获取资产数据的工厂类*/
	UFUNCTION(BlueprintCallable, Category = "UnrealEditorExtensions|ActorFactory")
		static UActorFactory* GetActorFactory(const FAssetData& AssetData, bool bRequireValidObject = false);

	/*获取资产类的AActor*/
	UFUNCTION(BlueprintCallable, Category = "UnrealEditorExtensions|ActorFactory")
		static AActor* GetDefaultActor(const FAssetData& AssetData);

	/*从资产中生成对象到视口*/
	UFUNCTION(BlueprintCallable, Category = "UnrealEditorExtensions|ActorFactory", meta = (WorldContext = "WorldContextObject"))
		static AActor* CreateActor(const UObject * WorldContextObject,const FAssetData& AssetData,FTransform SpawnTransform);

	/*获取鼠标是否在编辑器视口中*/
	UFUNCTION(BlueprintPure, Category = "UnrealEditorExtensions|ViewPort", meta = (WorldContext = "WorldContextObject"))
		static bool  IsMouseInMainViewport(const UObject* WorldContextObject, FVector &ZoomToPoint);

	/*获取BluePrint类的Description*/
	UFUNCTION(BlueprintPure, Category = "UnrealEditorExtensions|BluePrint")
		static bool  GetBlueprintDescription(const FAssetData& AssetData,FString &BlueprintDescription);

	/*获取资产的缩略图*/
	UFUNCTION(BlueprintPure, Category = "UnrealEditorExtensions|AssetThumbnail")
		static UTexture2D*  GetObjectThumbnail(const FAssetData& AssetData);

	/* 获取获取所有属性
	 * @param	Class				类。
	 */
	UFUNCTION(BlueprintCallable, Category = "UnrealEditorExtensions|Properties")
		static TArray<FString> GetAllProperties(UClass* Class);

	//从内容浏览器定位资产目标
	UFUNCTION(BlueprintCallable, Category = "UnrealEditorExtensions|Asset")
		static void FindAssetToBrowser(UObject* Asset);

	//迁移资产
	UFUNCTION(BlueprintCallable, Category = "UnrealEditorExtensions|Asset")
		static void ExecuteMigrateAsset(UObject* Asset);

	/* 创建动画资产
	 * @param	Animation				动画序列对象
	 * @param	AssetPath				资产保存的路径默认带/Game/直接填写后面路径即可
	 */
	UFUNCTION(BlueprintCallable, Category = "UnrealEditorExtensions|Asset")
		static void NewAnimAssetToDisk(UAnimSequence* Animation,FString AssetPath);

	/*这个资产是否为关卡类*/
	UFUNCTION(BlueprintPure, Category = "UnrealEditorExtensions|Asset")
		static bool  IsLevelFromAssetData(const FAssetData& AssetData);

	/*获取资产在编辑器上显示的名称*/
	UFUNCTION(BlueprintPure, Category = "UnrealEditorExtensions|Asset")
		static FLinearColor GetAssetClassDisplayColor(const FAssetData& AssetData);

	/* 从资产获取对应的类
	 * @param	Asset				任意资产
	 */
	UFUNCTION(BlueprintPure, Category = "UnrealEditorExtensions|Asset")
		static UClass* GetClassFormAsset(UObject* Asset);

	/* 从资产数据获取关卡中的WorldSetting
	 * @param	AssetData				任意资产数据
	 */
	UFUNCTION(BlueprintPure, Category = "UnrealEditorExtensions|Asset")
		static AWorldSettings* GetWorldSettingFromAssetData(const FAssetData& AssetData);

	/* 根据路径从资产管理器获取对应的资产如:/Game/../../
	 * @param	FilePath				Game开头的路径。
	 * @param	FileName				找到后输出对应的路径
	 */
	UFUNCTION(BlueprintCallable, Category = "UnrealEditorExtensions|Asset")
		static  UObject* GetAssetOfPath(const FString FilePath, FString& FileName);

	/* 设置关卡的游戏模式。可以从资产的关卡设置，也可以从当前关卡设置
	 * @param	CurrentLevel			是否设置WorldContextObject的关卡的默认模式
	 * @param	WorldSettings			资产关卡的WorldSetting。
	 * @param	GameMode				新的GM
	 * @param	GameState				新的GS
	 */
	UFUNCTION(BlueprintCallable, Category = "UnrealEditorExtensions|Setting", meta = (WorldContext = "WorldContextObject"))
		static bool SetWorldSettingGameModeGameState(bool CurrentLevel, const UObject* WorldContextObject, AWorldSettings* WorldSettings, TSubclassOf<AGameModeBase> GameMode, TSubclassOf<AGameStateBase> GameState);


	UFUNCTION(BlueprintCallable, Category = "UnrealEditorExtensions|Editor")
		static void CloseAllAreas();
	/* 运行编辑器蓝图或控件
	 * @param	Asset			资产名称
	 */
	UFUNCTION(BlueprintCallable, Category = "UnrealEditorExtensions|Editor")
		static void ExecuteRun(UObject* Asset);

	/* 在编辑器蓝图中使用。获取UE引擎的世界上下文
	 * @param	Return			返回UE引擎的世界上下文
	 */
	UFUNCTION(BlueprintPure, Category = "UnrealEditorExtensions|Editor")
		static UWorld* GetEngineWorldContextObject();
	
private:
	
};

