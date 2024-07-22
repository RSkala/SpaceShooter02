// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

//#include "CoreMinimal.h"
#include "Engine.h"
#include "Modules/ModuleManager.h"
#include "UnrealEd.h"

// Note: This file acts as the Precompiled Header for the module, so include
// anything possibly needed in most files here for faster compile times.

DECLARE_LOG_CATEGORY_EXTERN(LogSpaceShooter02Editor, All, All)

class FSpaceShooter02EditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
