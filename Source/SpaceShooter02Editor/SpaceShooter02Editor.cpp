// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpaceShooter02Editor.h"

#include "Modules/ModuleManager.h"

IMPLEMENT_GAME_MODULE(FSpaceShooter02EditorModule, SpaceShooter02Editor)

DEFINE_LOG_CATEGORY(LogSpaceShooter02Editor)

#define LOCTEXT_NAMESPACE "SpaceShooter02Editor"

void FSpaceShooter02EditorModule::StartupModule()
{
	UE_LOG(LogSpaceShooter02Editor, Warning, TEXT("SpaceShooter02Editor: Module Started"));
}

void FSpaceShooter02EditorModule::ShutdownModule()
{
	UE_LOG(LogSpaceShooter02Editor, Warning, TEXT("SpaceShooter02Editor: Module Shutdown"));
}

#undef LOCTEXT_NAMESPACE
