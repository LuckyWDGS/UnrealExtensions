#include "UnrealEditorExtensionsBPFLibrary.h"

#include "ActorFactories/ActorFactory.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetSelection.h"
#include "AssetToolsModule.h"
#include "AnimationEditorUtils.h"

#include "Blutility/Public/EditorUtilitySubsystem.h"
#include "Blutility/Classes/EditorUtilityWidgetBlueprint.h"
#include "Blutility/Classes/EditorUtilityWidget.h"
#include "Blutility/Public/IBlutilityModule.h"

#include "ContentBrowserModule.h"

#include <Engine/DataTable.h> 
#include "Editor.h"
#include "Editor/EditorEngine.h"


#include "Factories/AnimSequenceFactory.h"
#include "Framework/Commands/UICommandList.h"

#include "GameFramework/GameModeBase.h"
#include "GameFramework/WorldSettings.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/BlueprintPathsLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet2/DebuggerCommands.h"

#include "LevelEditor/Public/LevelEditor.h"

#include "UMGEditor/Public/WidgetBlueprint.h"
#include "UObject/SavePackage.h"
#include "UnrealEd/Public/LevelEditorViewport.h"
#include "UnrealEd/Public/FileHelpers.h"

#include "Internationalization/Regex.h"
#include "IImageWrapper.h" 
#include "IImageWrapperModule.h"

#include "Misc/FileHelper.h"

#include "IContentBrowserSingleton.h"

#include "ToolMenu.h"
#include "ToolMenus.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "Toolkits/AssetEditorToolkitMenuContext.h"

#include "Materials/MaterialInstance.h"

#include "ObjectTools.h"



UObject* UUnrealEditorExtensionsBPFLibrary::GetOpenObjecFromToolMenuContext(const FToolMenuContext& InContext)
{
    
    if (UAssetEditorToolkitMenuContext* ToolkitMenu = Cast<UAssetEditorToolkitMenuContext>(InContext.FindByClass(UAssetEditorToolkitMenuContext::StaticClass()))){
        const TArray< UObject* >*ArrayObject;
        ArrayObject=ToolkitMenu->Toolkit.Pin().Get()->GetObjectsCurrentlyBeingEdited();
        if (ArrayObject->Num() == 1){
            for (UObject* Object : (*ArrayObject)){
                return Object;
            }
        }
        return nullptr;
    }
    return nullptr;
}

TArray<FString> UUnrealEditorExtensionsBPFLibrary::GetSelectedFolders()
{
    TArray<FString> FolderPaths;
    FContentBrowserModule& ContentBrowserModule = FModuleManager::GetModuleChecked<FContentBrowserModule>("ContentBrowser");//通过模块名称获取对
    ContentBrowserModule.Get().GetSelectedFolders(FolderPaths);
    return FolderPaths;
}

TMap<FName, UToolMenu*> UUnrealEditorExtensionsBPFLibrary::GetAllMenu()
{
    //FindPropertyByName 获取经过UE反射的变量(针对UPROPERTY())。ContainerPtrToValuePtr模板类。转换成你需要的数据类型，传入对象
   auto AllMenus= UToolMenus::Get()->GetClass()->FindPropertyByName("Menus")->ContainerPtrToValuePtr<TMap<FName, UToolMenu*>>(UToolMenus::Get());

    return *AllMenus;
}

UActorFactory* UUnrealEditorExtensionsBPFLibrary::GetActorFactory(const FAssetData& AssetData, bool bRequireValidObject)
{
    UObject* Asset = NULL;
    if (bRequireValidObject) {
        Asset = AssetData.GetClass()->GetDefaultObject();
    }
    else {
        Asset = AssetData.GetAsset();
    }
    return FActorFactoryAssetProxy::GetFactoryForAssetObject(Asset);
}

AActor* UUnrealEditorExtensionsBPFLibrary:: GetDefaultActor(const FAssetData& AssetData)
{
    UActorFactory* ActorFactory = GetActorFactory(AssetData);
    if (ActorFactory) {
        return ActorFactory->GetDefaultActor(AssetData);
    }
    return NULL;
}

AActor* UUnrealEditorExtensionsBPFLibrary::CreateActor(const UObject* WorldContextObject, const FAssetData& AssetData, FTransform SpawnTransform)
{
    // 开始一个新的事务
    FScopedTransaction Transaction(FText::FromString("New Creat Actor"));
    UActorFactory* ActorFactory = GetActorFactory(AssetData);
    if (ActorFactory) {
        return ActorFactory->CreateActor(AssetData.GetAsset(), WorldContextObject->GetWorld()->GetCurrentLevel(), SpawnTransform);
    }
    //取消当前事务
    Transaction.Cancel();
    return NULL;
}

bool UUnrealEditorExtensionsBPFLibrary::IsMouseInMainViewport(const UObject* WorldContextObject, FVector &ZoomToPoint)
{
    UWorld* World = WorldContextObject->GetWorld();

    FViewport* Viewport = GEditor->GetActiveViewport();

    if (!Viewport){
        return false;
    }
    FVector2D MousePosition = FSlateApplication::Get().GetCursorPos();

    // 获取世界视口的屏幕区域 
    const FVector2D ViewportPosition = Viewport->ViewportToVirtualDesktopPixel(FVector2D(0, 0));
    const FVector2D ViewportSize(Viewport->GetSizeXY());
    // 检查鼠标位置是否在世界视口的屏幕区域内
    if (MousePosition.X >= ViewportPosition.X && MousePosition.X <= ViewportPosition.X + ViewportSize.X &&
        MousePosition.Y >= ViewportPosition.Y && MousePosition.Y <= ViewportPosition.Y + ViewportSize.Y) {
        // 鼠标悬停在世界视口上
        // 在这里可以执行相应的逻辑
        FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(
            GCurrentLevelEditingViewportClient->Viewport,
            GCurrentLevelEditingViewportClient->GetWorld()->Scene,
            GCurrentLevelEditingViewportClient->EngineShowFlags)
            .SetRealtimeUpdate(true));

        FSceneView* SceneView = GCurrentLevelEditingViewportClient->CalcSceneView(&ViewFamily);

        if (SceneView) {
            FIntPoint MouseIntPosition;
            FVector WorldOrigin;
            FVector WorldDirection;
            GCurrentLevelEditingViewportClient->Viewport->GetMousePos(MouseIntPosition);

            SceneView->DeprojectFVector2D(MouseIntPosition, WorldOrigin, WorldDirection);

            FHitResult HitResult;

            FCollisionQueryParams LineParams(SCENE_QUERY_STAT(FocusOnPoint), true);

            if (GCurrentLevelEditingViewportClient->GetWorld()->LineTraceSingleByObjectType(HitResult, WorldOrigin, WorldOrigin + WorldDirection * HALF_WORLD_MAX, FCollisionObjectQueryParams::AllObjects, LineParams)) {
                ZoomToPoint = HitResult.ImpactPoint;
                return true;
            }
            ZoomToPoint = WorldOrigin + WorldDirection * 500;
            return true;
        }
    }
    return false;

}

bool UUnrealEditorExtensionsBPFLibrary::GetBlueprintDescription(const FAssetData& AssetData, FString& BlueprintDescription)
{
    
    UBlueprint* Blueprint = Cast<UBlueprint>(AssetData.GetAsset());
    if (Blueprint) {
        BlueprintDescription = Blueprint->BlueprintDescription;
        return true;
    }
    BlueprintDescription.Empty();
    return false;
}

UTexture2D* UUnrealEditorExtensionsBPFLibrary::GetObjectThumbnail(const FAssetData& AssetData)
{       
        UTexture2D* Texture = nullptr;
        UObject* AssetObject = AssetData.GetAsset();
        if (!AssetObject) {
            return NULL;
        }
        FObjectThumbnail* ObjThumnail = ThumbnailTools::GenerateThumbnailForObjectToSaveToDisk(AssetObject);
        if (!ObjThumnail){

            return NULL;
        }
        TArray<uint8> ThumnailDatat = ObjThumnail->GetUncompressedImageData();

        IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
        TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
        if (ImageWrapper.IsValid()){
                Texture = UTexture2D::CreateTransient(ObjThumnail->GetImageWidth(), ObjThumnail->GetImageHeight(), PF_B8G8R8A8);
                if (Texture != nullptr)
                {
                    void* TextureData = Texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
                    FMemory::Memcpy(TextureData, ThumnailDatat.GetData(), ThumnailDatat.Num());
                    Texture->PlatformData->Mips[0].BulkData.Unlock();
                    Texture->UpdateResource();
                }
        }
        return Texture;
}

TArray<FString> UUnrealEditorExtensionsBPFLibrary::GetAllProperties(UClass* Class)
{
    TArray<FString> Ret;
    if (Class != nullptr) {
        for (TFieldIterator<FProperty> It(Class); It; ++It) {
            FProperty* Property = *It;
            if (Property->HasAnyPropertyFlags(EPropertyFlags::CPF_Edit)) {
                Ret.Add(Property->GetName());
            }
        }
    }
    return Ret;
}

void UUnrealEditorExtensionsBPFLibrary::FindAssetToBrowser(UObject* Object)
{
    TArray<UObject*>ObjectsToSyncTo;
    if (Object) {
        ObjectsToSyncTo.Add(Object);
        GEditor->SyncBrowserToObjects(ObjectsToSyncTo, true);
    }
}

void UUnrealEditorExtensionsBPFLibrary::ExecuteMigrateAsset(UObject* Asset)
{
    if (Asset) {
        TArray<FName> PackageNames; 
        FString AssetPathName = Asset->GetPathName();
        PackageNames.Add(*AssetPathName);
        FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");
        AssetToolsModule.Get().MigratePackages(PackageNames);

    }
}

void UUnrealEditorExtensionsBPFLibrary::NewAnimAssetToDisk(UAnimSequence* Animation, FString AssetPath)
{
    //增加默认路径
    AssetPath = "/Game/" + AssetPath;
    if (UBlueprintPathsLibrary::FileExists(AssetPath))
    {
        UE_LOG(LogTemp, Warning, TEXT("文件存在"));
        return;
    }
    FString AssetName = FPaths::GetBaseFilename(AssetPath);

    // Create a new package for the duplicated AnimSequence object
    UPackage* Package = CreatePackage( *AssetPath);

    // Duplicate the source AnimSequence object
    UAnimSequence* DuplicatedAnimSequence = DuplicateObject<UAnimSequence>(Animation, Package);

    // Rename the duplicated AnimSequence object
    DuplicatedAnimSequence->Rename(*AssetName, Package);

    // Mark the package as dirty and notify the asset registry of the new asset
    Package->SetDirtyFlag(true);
    FAssetRegistryModule::AssetCreated(DuplicatedAnimSequence);

    // Save the duplicated AnimSequence object to disk
    FString PackageFileName = FPackageName::LongPackageNameToFilename(Package->GetName(), FPackageName::GetAssetPackageExtension());
    UPackage::SavePackage(Package, DuplicatedAnimSequence, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *PackageFileName, GError, nullptr, true, true, SAVE_NoError);
}

bool UUnrealEditorExtensionsBPFLibrary::IsLevelFromAssetData(const FAssetData& AssetData)
{
    UWorld* World = Cast<UWorld>(AssetData.GetAsset());
    if (World) {
        return true;
    }
    return false;
}

FLinearColor UUnrealEditorExtensionsBPFLibrary::GetAssetClassDisplayColor(const FAssetData& AssetData)
{
    UClass* AssetClass = AssetData.GetClass();
    if (AssetClass) {
        // 获取AssetTools模块
        FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");

        // 获取AssetTools模块的AssetTypeActions
        IAssetTools& AssetTools = AssetToolsModule.Get();

        IAssetTypeActions* AssetTypeActions=AssetTools.GetAssetTypeActionsForClass(AssetClass).Pin().Get();
        AssetTypeActions->GetTypeColor();

        return  AssetTypeActions->GetTypeColor();
    }
    return FLinearColor();
}

UClass* UUnrealEditorExtensionsBPFLibrary::GetClassFormAsset(UObject* Asset)
{
    if (Asset) {
        return Asset->GetClass();
    }
    return nullptr;
}

AWorldSettings* UUnrealEditorExtensionsBPFLibrary::GetWorldSettingFromAssetData(const FAssetData& AssetData)
{
    if (UWorld* World = Cast<UWorld>(AssetData.GetAsset())) {
        return World->GetWorldSettings();
    }
    return nullptr;
}

UObject* UUnrealEditorExtensionsBPFLibrary::GetAssetOfPath(const FString FilePath, FString& FileName)
{
    if (FilePath.Len() > 0) {
        FName PackgePath = *FPaths::GetPath(FilePath);
        TArray<FAssetData> OutAssetData;
        const IAssetRegistry& AssetRegisty = IAssetRegistry::GetChecked();
        AssetRegisty.GetAssetsByPath(PackgePath, OutAssetData, false, false);//遍历索引目录下的所有资产
        for (auto i : OutAssetData) {
            if (i.PackageName.ToString() == FilePath) {
                FileName = i.PackageName.ToString();
                return i.GetAsset();
            }
        }
        return nullptr;
    }
    return nullptr;
}

void UUnrealEditorExtensionsBPFLibrary::SetAssetParent(const FAssetData& AssetData, TSubclassOf<UObject>NewParentClass)
{
    if (AssetData.GetAsset()) {
        UBlueprint* Blueprint = Cast<UBlueprint>(AssetData.GetAsset());
        if (Blueprint) {
            Blueprint->ParentClass = NewParentClass;
            Blueprint->MarkPackageDirty();
            return;
        }
    }
}

bool UUnrealEditorExtensionsBPFLibrary::SetWorldSettingGameModeGameState(bool CurrentLevel, const UObject* WorldContextObject, AWorldSettings* WorldSettings, TSubclassOf<AGameModeBase> GameMode, TSubclassOf<AGameStateBase> GameState)
{
    AWorldSettings* WolrdSettingActor = nullptr;
    switch (CurrentLevel)
    {
    case true:
        WolrdSettingActor = WorldContextObject->GetWorld()->GetWorldSettings();
        break;
    case false:
        WolrdSettingActor = WorldSettings;
        break;
    }
    if (WolrdSettingActor) {
        GameMode.GetDefaultObject()->GameStateClass = GameState;
        WolrdSettingActor->DefaultGameMode = GameMode;
        return true;
    }
    return false;
}

void UUnrealEditorExtensionsBPFLibrary::CloseAllAreas()
{
    FLevelEditorModule& LevelEditorModule = FModuleManager::GetModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
    TSharedPtr<FTabManager> LevelEditorTabManager = LevelEditorModule.GetLevelEditorTabManager();
    LevelEditorTabManager.Get()->CloseAllAreas();
 
}

void UUnrealEditorExtensionsBPFLibrary::ExecuteRun(UObject* Asset)
{
        UEditorUtilitySubsystem* EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();
        if (EditorUtilitySubsystem) {
            EditorUtilitySubsystem->TryRun(Asset);
        }
           
}

UWorld* UUnrealEditorExtensionsBPFLibrary::GetEngineWorldContextObject()
{
    FWorldContext* WorldContext = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);
    if (WorldContext && WorldContext->World())
    {
        return WorldContext->World();
    }
    return nullptr;
}

void UUnrealEditorExtensionsBPFLibrary::SelectedSceneActor(AActor* Actor, bool IsSelected)
{
    GEditor->SelectActor(Actor, IsSelected, true);
}

UMaterialInstanceConstant* UUnrealEditorExtensionsBPFLibrary::AssetDataToMaterialInstanceConstant(const FAssetData& AssetData)
{
    if (AssetData.GetAsset()) {
        UMaterialInstanceConstant *MaterialInstanceConstant = Cast<UMaterialInstanceConstant>(AssetData.GetAsset());
            return MaterialInstanceConstant;
    }
    return nullptr;
}





