// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "SpaceShooterLevelScriptActor.generated.h"

UCLASS()
class SPACESHOOTER02_API ASpaceShooterLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

protected:
	// If true, skips loading the main menu and goes right into gameplay
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSkipMainMenuLoad = false; // TODO: Move into GameState
};
