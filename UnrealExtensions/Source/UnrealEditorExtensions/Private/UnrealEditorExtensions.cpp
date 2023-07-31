// Copyright 2022 Dreamingpoet All Rights Reserved.
#include "UnrealEditorExtensions.h"
#include "UnrealEditorExtensionsBPFLibrary.h"
#define LOCTEXT_NAMESPACE "FUnrealEditorExtensionsModule"

void FUnrealEditorExtensionsModule::StartupModule()
{
	GEditor->GetTimerManager()->SetTimer(TimerHandle, FTimerDelegate::CreateRaw(this,&FUnrealEditorExtensionsModule::InitEditorBlueprint), 3, false);
	
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FUnrealEditorExtensionsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

void FUnrealEditorExtensionsModule::InitEditorBlueprint()
{
	FString Path = L"/Game/Extender/SimuEditorUtilityBlueprint.SimuEditorUtilityBlueprint";
	FString Return;
	UObject* Asset = UUnrealEditorExtensionsBPFLibrary::GetAssetOfPath(Path, Return);
	if (Asset) {
		UUnrealEditorExtensionsBPFLibrary::ExecuteRun(UUnrealEditorExtensionsBPFLibrary::GetAssetOfPath(Path, Return));
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUnrealEditorExtensionsModule, UnrealEditorExtensions)