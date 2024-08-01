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

	void StartMainMenu();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> MainMenuScreenClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> PlayerShipSelectScreenClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> GameOverScreenClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TObjectPtr<class UMainMenuScreen> MainMenuScreen;

	// If true, skips loading the main menu and goes right into gameplay
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSkipMainMenuLoad = false;
};
