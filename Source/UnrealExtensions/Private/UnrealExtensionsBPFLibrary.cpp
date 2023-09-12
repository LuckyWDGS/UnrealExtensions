﻿#include "UnrealExtensionsBPFLibrary.h"
#include "AssetRegistry/IAssetRegistry.h"

#include "Components/SceneCaptureComponent2D.h"//SceneCaptureComponent2D组件的头文件
#include "Components/Widget.h"
#include <Engine/DataTable.h> 
#include "EngineUtils.h"
#include "Engine/Texture2D.h" 

#include "Engine/TextureRenderTarget2D.h"//TextureRenderTarget2D组件的头文件

#include "GameFramework/WorldSettings.h"

#include "IImageWrapper.h" 
#include "IImageWrapperModule.h"
#include "Internationalization/Regex.h"
#include "ImageUtils.h"

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


#include "Misc/FileHelper.h"

AWorldSettings* UUnrealExtensionsBPFLibrary::GetWorldSetting(const UObject* WorldContextObject)
{

    if (UWorld* World = Cast<UWorld>(WorldContextObject->GetWorld())) {
        return World->GetWorldSettings();
    }
    return nullptr;
}

TArray<FString> UUnrealExtensionsBPFLibrary::LoadTXT(const FString TXTPath, FString& OutTXT)
{
    TArray<FString>OutArray;
    //查找文本是否存在
    if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*TXTPath))
    {
        FString str = TXTPath + "文件不存在！";
        UE_LOG(LogTemp, Warning, TEXT("%s,文件不存在！"), *str);
        return OutArray;
    }

    //读取文本
    FFileHelper::LoadFileToString(OutTXT, *TXTPath);
    //对文本逐行处理 存入OutArray
    FFileHelper::LoadFileToStringArray(OutArray, *TXTPath);
    return OutArray;

}

bool UUnrealExtensionsBPFLibrary::WriteTXT(const FString SaveString, const FString FileName)
{
    return FFileHelper::SaveStringToFile(SaveString, *FileName);
}

void UUnrealExtensionsBPFLibrary::GetStatData(const FString InPath, FFileData& FileData)
{

    FFileStatData  FFileData = IFileManager::Get().GetStatData(*InPath);
    FileData.CreationTime = FFileData.CreationTime;
    FileData.AccessTime = FFileData.AccessTime;
    FileData.ModificationTime = FFileData.ModificationTime;
    FileData.FileSize = FFileData.FileSize;
    FileData.IsDirectory = FFileData.bIsDirectory;
    FileData.IsReadOnly = FFileData.bIsReadOnly;
    FileData.bIsValid = FFileData.bIsValid;

    if (FileData.FileSize == -1) {
        int64 FolderSize = 0;
        TArray<FString>Files;
        IFileManager::Get().FindFilesRecursive(Files, *InPath, TEXT("*"), true, false);
        for (auto i : Files) {
            if (IFileManager::Get().GetStatData(*i).FileSize > 0) {
                FolderSize = FolderSize + IFileManager::Get().GetStatData(*i).FileSize;
            }
        }
        FileData.FileSize = FolderSize;
    }
}

bool UUnrealExtensionsBPFLibrary::IsWithEditor()
{
    {
#if WITH_EDITOR
        return true;
#else
        return false;
#endif
    }
}

bool UUnrealExtensionsBPFLibrary::ExploreFolder(const FString InPath)
{
    if (FPaths::DirectoryExists(InPath)){
        FPlatformProcess::ExploreFolder(*InPath);
        return true;
    }
    GEngine->AddOnScreenDebugMessage((uint64)-1, 5, FColor(99,99,199), InPath+"--->>> 文件夹不存在");
    return false;
}

bool UUnrealExtensionsBPFLibrary::FindDirectory(const FString InPath,TArray<FString>&OutPutFolder,bool RemovePath)
{
    class FFileMatch : public IPlatformFile::FDirectoryVisitor
    {
    public:
        TArray<FString>& Result;
        FString WildCard;
        bool bFiles;
        bool bDirectories;
        FFileMatch(TArray<FString>& InResult, const FString& InWildCard, bool bInFiles, bool bInDirectories)
            : Result(InResult)
            , WildCard(InWildCard)
            , bFiles(bInFiles)
            , bDirectories(bInDirectories)
        {
        }
        virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory)
        {
            if ((bIsDirectory && bDirectories) || (!bIsDirectory && bFiles))
            {
                const FString Filename = FPaths::GetCleanFilename(FilenameOrDirectory);
                if (Filename.MatchesWildcard(WildCard))
                {
                    new(Result) FString(Filename);
                }
            }
            return true;
        }
    };
    FFileMatch FileMatch(OutPutFolder, "*", false, true);
    if (RemovePath){
        IFileManager::Get().IterateDirectory(*InPath, FileMatch);
        return OutPutFolder.Num() < 0;
    }
    TArray<FString> PathDirectory;
    IFileManager::Get().IterateDirectory(*InPath, FileMatch);
    for (auto i : OutPutFolder){
        PathDirectory.Add(InPath +"/"+ i);
    }
    OutPutFolder = PathDirectory;
    return PathDirectory.Num() < 0;
}

bool UUnrealExtensionsBPFLibrary::OpenbyFile(const FString InPath)
{
    return FWindowsPlatformMisc::OsExecute(TEXT("open"), *InPath);
}

bool UUnrealExtensionsBPFLibrary::RegexMatch(const FString& Str, const FString& Pattern, TArray<FString>& Result)
{
    FRegexPattern MatherPatter(Pattern);
    FRegexMatcher Matcher(MatherPatter, Str);
    while (Matcher.FindNext()){
        Result.Add(Matcher.GetCaptureGroup(0));

    }
    return Result.Num() == 0 ? false : true;
}

FString UUnrealExtensionsBPFLibrary::GetLevelPath(const UObject* WorldContextObject)
{
    FString LevelPath;
    if (UWorld* World = Cast<UWorld>(WorldContextObject->GetWorld())) {
        LevelPath = FPaths::GetBaseFilename(World->GetLevel(0)->GetPathName(), false);
        return LevelPath;
    }
    return LevelPath;
}

static TSharedPtr<IImageWrapper> GetImageWrapperByExtention(const FString InImagePath)
{
    IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
    if (InImagePath.EndsWith(".png"))
    {
        return ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
    }
    else if (InImagePath.EndsWith(".jpg") || InImagePath.EndsWith(".jpeg"))
    {
        return ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
    }
    else if (InImagePath.EndsWith(".bmp"))
    {
        return ImageWrapperModule.CreateImageWrapper(EImageFormat::BMP);
    }
    else if (InImagePath.EndsWith(".ico"))
    {
        return ImageWrapperModule.CreateImageWrapper(EImageFormat::ICO);
    }
    else if (InImagePath.EndsWith(".exr"))
    {
        return ImageWrapperModule.CreateImageWrapper(EImageFormat::EXR);
    }
    else if (InImagePath.EndsWith(".icns"))
    {
        return ImageWrapperModule.CreateImageWrapper(EImageFormat::ICNS);
    }
    return nullptr;
}

UTexture2D* UUnrealExtensionsBPFLibrary::LoadTexture2D(const FString& ImagePath, bool& IsValid, int32& OutWidth, int32& OutHeight)
{
    UTexture2D* Texture = nullptr;
    IsValid = false;
    if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*ImagePath))
    {
        return nullptr;
    }
    TArray<uint8> CompressedData;
    if (!FFileHelper::LoadFileToArray(CompressedData, *ImagePath))
    {
        return nullptr;
    }
    TSharedPtr<IImageWrapper> ImageWrapper = GetImageWrapperByExtention(ImagePath);
    if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(CompressedData.GetData(), CompressedData.Num()))
    {
        TArray<uint8> UncompressedRGBA;
        if (ImageWrapper->GetRaw(ERGBFormat::RGBA, 8, UncompressedRGBA))
        {
            Texture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_R8G8B8A8);
            if (Texture != nullptr)
            {
                IsValid = true;
                OutWidth = ImageWrapper->GetWidth();
                OutHeight = ImageWrapper->GetHeight();
                void* TextureData = Texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
                FMemory::Memcpy(TextureData, UncompressedRGBA.GetData(), UncompressedRGBA.Num());
                Texture->PlatformData->Mips[0].BulkData.Unlock();
                Texture->UpdateResource();
            }
        }
    }
    return Texture;

}

bool UUnrealExtensionsBPFLibrary::ConvertAndResizeImage(const FString& ImagePath, const FString& SaveFilePath, int32 NewWidth, int32 NewHeight,int32 Quality)
{
        TArray<uint8> InputImageData;
        TArray<uint8> OutputImageData;
       
        FFileHelper::LoadFileToArray(InputImageData, *ImagePath);
        IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
        TSharedPtr<IImageWrapper> ImageWrapper = GetImageWrapperByExtention(ImagePath);
     
        if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(InputImageData.GetData(), InputImageData.Num())){
            TArray<uint8> UncompressedRGBA;
            if (ImageWrapper->GetRaw(ERGBFormat::RGBA, 8, UncompressedRGBA)){
                TSharedPtr<IImageWrapper> ConvertImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
                if (NewWidth != 0 && NewHeight != 0)
                {
                    TArrayView<FColor> OutColors;
                    FImageUtils::ImageResize(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), (TArrayView<FColor>&)UncompressedRGBA, NewWidth, NewHeight, OutColors, true);
                    ConvertImageWrapper->SetRaw(OutColors.GetData(), OutColors.Num() * 4, NewWidth, NewHeight, ERGBFormat::RGBA, 8);
                }
                else {
                    ConvertImageWrapper->SetRaw(UncompressedRGBA.GetData(), UncompressedRGBA.Num(), ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), ERGBFormat::RGBA, 8);
                }
                OutputImageData = ConvertImageWrapper->GetCompressed(Quality);
                FString FilName = SaveFilePath + ".jpg";
                FFileHelper::SaveArrayToFile(OutputImageData, *FilName);
                return true;
            }
        }
        return false;
}

UObject* UUnrealExtensionsBPFLibrary::GetBlueprintFormAssetData(const FAssetData& AssetData)
{
    if (AssetData.GetAsset()) {

        UBlueprint* Blueprint = Cast<UBlueprint>(AssetData.GetAsset());
        if (Blueprint) {
            return Blueprint->GeneratedClass->GetDefaultObject();
        }
    }
    return nullptr;
}

void UUnrealExtensionsBPFLibrary::ScreenShot(USceneCaptureComponent2D* CaptureComponent2D, const FString& SavePath)
{
    if (CaptureComponent2D&& CaptureComponent2D->TextureTarget) {
    auto Lab = [=]()
    {
        FTextureRenderTargetResource* TextureRenderTargetResource = CaptureComponent2D->TextureTarget->GameThread_GetRenderTargetResource();
        int32 Width = CaptureComponent2D->TextureTarget->SizeX;
        int32 Height = CaptureComponent2D->TextureTarget->SizeY;
        TArray<FColor> OutData;
        TextureRenderTargetResource->ReadPixels(OutData);

        ColorToImage(SavePath, OutData, Width, Height);
    };

    FTimerHandle TimerHandle;
    GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport)->World()->GetTimerManager().SetTimer(TimerHandle, Lab, 0.001f, false, 0);
    return;
    }

    UE_LOG(LogTemp, Error, TEXT("CaptureComponent2D == Null Or CaptureComponent2D->TextureTarget == Null "));
}
   

void UUnrealExtensionsBPFLibrary::ColorToImage(const FString& InImagePath, TArray<FColor> InColor, int32 InWidth, int32 InHight)
{
    IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>("ImageWrapper");
    FString	Ex = FPaths::GetExtension(InImagePath);
    if (Ex.Equals(TEXT("jpg"), ESearchCase::IgnoreCase) || Ex.Equals(TEXT("jpeg"), ESearchCase::IgnoreCase))
    {
        TSharedPtr<IImageWrapper>ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
      
        if (ImageWrapper->SetRaw(InColor.GetData(), InColor.GetAllocatedSize(), InWidth, InHight, ERGBFormat::BGRA, 8))
        {
            FFileHelper::SaveArrayToFile(ImageWrapper->GetCompressed(100), *InImagePath);
        }
    }
    else
    {
        TArray<uint8>OutPNG;

        for (FColor& color : InColor)
        {
            color.A = 255;
        }
        FImageUtils::CompressImageArray(InWidth, InHight, InColor, OutPNG);
        FFileHelper::SaveArrayToFile(OutPNG, *InImagePath);

    }

}

USoundWave* UUnrealExtensionsBPFLibrary::SoundForByteData(TArray<uint8> RawWaveData, FString SavePath, bool IsSave)
{
    if (IsSave) {
        FFileHelper::SaveArrayToFile(RawWaveData, *SavePath);
    }
    FWaveModInfo WaveInfo;
    if (!WaveInfo.ReadWaveInfo(RawWaveData.GetData(), RawWaveData.Num()))
    {
        return nullptr;
    }

    USoundWave* Sound = NewObject<USoundWave>();

    int32 ChannelCount = (int32)*WaveInfo.pChannels;
    check(ChannelCount > 0);

    int32 SizeOfSample = (*WaveInfo.pBitsPerSample) / 8;

    int32 NumSamples = WaveInfo.SampleDataSize / SizeOfSample;
    int32 NumFrames = NumSamples / ChannelCount;

    Sound->RawData.Lock(LOCK_READ_WRITE);
    void* LockedData = Sound->RawData.Realloc(RawWaveData.Num());
    FMemory::Memcpy(LockedData, RawWaveData.GetData(), RawWaveData.Num());
    Sound->RawData.Unlock();

    Sound->Duration = (float)NumFrames / *WaveInfo.pSamplesPerSec;
    Sound->SetSampleRate(*WaveInfo.pSamplesPerSec);
    Sound->NumChannels = ChannelCount;
    Sound->TotalSamples = *WaveInfo.pSamplesPerSec * Sound->Duration;

    return Sound;
}



