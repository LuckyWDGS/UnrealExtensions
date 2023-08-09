// Copyright 2022 Dreamingpoet All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FUnrealEditorExtensionsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;


	FTimerHandle TimerHandle;
	void InitEditorBlueprint();

};
