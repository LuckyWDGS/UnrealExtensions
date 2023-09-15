// Copyright 2022 Dreamingpoet All Rights Reserved.
#include "UnrealEditorExtensions.h"
#include "UnrealEditorExtensionsBPFLibrary.h"
#define LOCTEXT_NAMESPACE "FUnrealEditorExtensionsModule"

void FUnrealEditorExtensionsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FUnrealEditorExtensionsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUnrealEditorExtensionsModule, UnrealEditorExtensions)