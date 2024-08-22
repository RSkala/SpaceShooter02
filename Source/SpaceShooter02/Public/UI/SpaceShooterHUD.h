// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SpaceShooterHUD.generated.h"

UCLASS()
class SPACESHOOTER02_API ASpaceShooterHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnGameStarted();
	
	UFUNCTION()
	void OnGameEnded(
		int32 FinalScore,
		int32 SelectedShipSpriteIndex,
		int32 NumEnemiesDefeated,
		int32 NumScoreMultipliersCollected,
		int32 NumEnemiesDefeatedWithBoost,
		int32 NumProjectilesFired,
		int32 CurrentScoreMultiplier);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> GameplayScreenClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UGameplayScreen> GameplayScreen;
};
