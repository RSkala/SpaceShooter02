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

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> GameplayScreenClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UGameplayScreen> GameplayScreen;
};
