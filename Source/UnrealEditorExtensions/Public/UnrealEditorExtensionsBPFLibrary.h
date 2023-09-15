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

	/* 根据工具菜单上下文获取打开的对象
	 * @param	FToolMenuContext		工具菜单上下文。
	 * @param	return					返回当前工具菜单上下文的使用者。
	 */
	UFUNCTION(BlueprintCallable, Category = "UnrealEditorExtensions|Path")
		static UObject* GetOpenObjecFromToolMenuContext(const FToolMenuContext& InContext);

	//获取内容浏览器选中的文件夹
	UFUNCTION(BlueprintPure, Category = "UnrealEditorExtensions|ContentBrowser")
		static TArray<FString> GetSelectedFolders();

	//获取所有的工具菜单栏
	UFUNCTION(BlueprintCallable, Category = "UnrealEditorExtensions|Tool Menus")
		static TMap<FName, UToolMenu*> GetAllMenu();

	/* 获取资产数据的工厂类
	 * @param	FAssetData				资产数据。
	 * @param	bRequireValidObject		要求有效对象。
	 */
	UFUNCTION(BlueprintCallable, Category = "UnrealEditorExtensions|ActorFactory")
		static UActorFactory* GetActorFactory(const FAssetData& AssetData, bool bRequireValidObject = false);
	
	/* 获取资产数据的默认对象
	 * @param	FAssetData				资产数据。
	 * @param	return					返回一个A类对象。
	 */
	UFUNCTION(BlueprintCallable, Category = "UnrealEditorExtensions|ActorFactory")
		static AActor* GetDefaultActor(const FAssetData& AssetData);

	/* Editor从资产数据中生成对象到关卡视频
	 * @param	FAssetData				资产数据。
	 * @param	SpawnTransform			生成的变换。
	 */
	UFUNCTION(BlueprintCallable, Category = "UnrealEditorExtensions|ActorFactory", meta = (WorldContext = "WorldContextObject"))
		static AActor* CreateActor(const UObject * WorldContextObject,const FAssetData& AssetData,FTransform SpawnTransform);

	/* 获取鼠标是否在编辑器视口中
	 * @param	ZoomToPoint				屏幕坐标。
	 */
	UFUNCTION(BlueprintPure, Category = "UnrealEditorExtensions|ViewPort", meta = (WorldContext = "WorldContextObject"))
		static bool  IsMouseInMainViewport(const UObject* WorldContextObject, FVector &ZoomToPoint);

	/* 获取BluePrint类的Description
	 * @param	AssetData				资产数据。
	 * @param	BlueprintDescription	描述信息
	 */
	UFUNCTION(BlueprintPure, Category = "UnrealEditorExtensions|BluePrint")
		static bool  GetBlueprintDescription(const FAssetData& AssetData,FString &BlueprintDescription);

	/* 获取资产的缩略图
	 * @param	AssetData				资产数据。
	 * @param	return					返回一个Texture2D
	 */
	UFUNCTION(BlueprintPure, Category = "UnrealEditorExtensions|Thumbnail")
		static UTexture2D*  GetObjectThumbnail(const FAssetData& AssetData);

	/* 获取类的所有属性
	 * @param	Class				类。
	 */
	UFUNCTION(BlueprintCallable, Category = "UnrealEditorExtensions|Properties")
		static TArray<FString> GetAllProperties(UClass* Class);
/*************************************************资产操作*************************************************************/
	
	/* 获取资产的缩略图
	 * @param	Asset					需要定位的资产。
	 */
	UFUNCTION(BlueprintCallable, Category = "UnrealEditorExtensions|Asset")
		static void FindAssetToBrowser(UObject* Asset);

	/* 迁移资产
	 * @param	Asset					需要迁移的资产。
	 */
	UFUNCTION(BlueprintCallable, Category = "UnrealEditorExtensions|Asset")
		static void ExecuteMigrateAsset(UObject* Asset);

	/* 创建动画资产
	 * @param	Animation				动画序列对象
	 * @param	AssetPath				资产保存的路径默认带/Game/直接填写后面路径即可
	 */
	UFUNCTION(BlueprintCallable, Category = "UnrealEditorExtensions|Asset")
		static void NewAnimAssetToDisk(UAnimSequence* Animation,FString AssetPath);

	/* 这个资产是否为关卡类
	 * @param	AssetData				资产数据
	 */
	UFUNCTION(BlueprintPure, Category = "UnrealEditorExtensions|Asset")
		static bool  IsLevelFromAssetData(const FAssetData& AssetData);

	/* 获取资产类显示颜色
	 * @param	AssetData				资产数据
	 */
	UFUNCTION(BlueprintPure, Category = "UnrealEditorExtensions|Asset")
		static FLinearColor GetAssetClassDisplayColor(const FAssetData& AssetData);

	/* 从资产获取对应的类
	 * @param	Asset				Object资产对象
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

	/* 重设资产的父类
	 * @param	AssetData				需要改变父类的资产数据
	 * @param	NewParentClass			新的父类
	 */
	UFUNCTION(BlueprintCallable, Category = "UnrealEditorExtensions|Asset")
		static void SetAssetParent(const FAssetData& AssetData, TSubclassOf<UObject>NewParentClass);

	/* 设置关卡的游戏模式。可以从资产的关卡设置，也可以从当前关卡设置
	 * @param	CurrentLevel			是否设置WorldContextObject的关卡的默认模式
	 * @param	WorldSettings			资产关卡的WorldSetting。
	 * @param	GameMode				新的GM
	 * @param	GameState				新的GS
	 */
	UFUNCTION(BlueprintCallable, Category = "UnrealEditorExtensions|Setting", meta = (WorldContext = "WorldContextObject"))
		static bool SetWorldSettingGameModeGameState(bool CurrentLevel, const UObject* WorldContextObject, AWorldSettings* WorldSettings, TSubclassOf<AGameModeBase> GameMode, TSubclassOf<AGameStateBase> GameState);

/******************************************************************编辑器工具类函数*******************************************************/
	/*关闭所有区域
	 */
	UFUNCTION(BlueprintCallable, Category = "UnrealEditorExtensions|Editor")
		static void CloseAllAreas();

	/* 运行编辑器蓝图或控件
	 * @param	Asset			资产
	 */
	UFUNCTION(BlueprintCallable, Category = "UnrealEditorExtensions|Editor")
		static void ExecuteRun(UObject* Asset);

	/* 在编辑器蓝图中使用。获取UE引擎的世界上下文
	 * @param	Return			返回UE引擎的世界上下文
	 */
	UFUNCTION(BlueprintPure, Category = "UnrealEditorExtensions|Editor")
		static UWorld* GetEngineWorldContextObject();
/******************************************************************场景视口工具类函数*******************************************************/
	
	/* 选定场景中的actor类
	 * @param	Actor			设置要操作的Actor
	 * @param	IsSelected		是否要在场景内选中此Actor
	 */
	UFUNCTION(BlueprintCallable, Category = "UnrealEditorExtensions|Editor")
		static void SelectedSceneActor(AActor* Actor,bool IsSelected=true);
private:
	
};

