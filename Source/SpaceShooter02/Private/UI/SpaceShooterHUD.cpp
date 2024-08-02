// Copyright 2024 Richard Skala

#include "UI/SpaceShooterHUD.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "SpaceShooterGameState.h"
#include "UI/GameplayScreen.h"

void ASpaceShooterHUD::BeginPlay()
{
	Super::BeginPlay();

	if (ensure(GameplayScreenClass != nullptr))
	{
		if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
		{
			GameplayScreen = Cast<UGameplayScreen>(UWidgetBlueprintLibrary::Create(GetWorld(), GameplayScreenClass, PlayerController));
			ensure(GameplayScreen != nullptr);
		}
	}

	// Listen for Game Start and Game Ended events
	ASpaceShooterGameState::OnGameStarted.AddUniqueDynamic(this, &ThisClass::OnGameStarted);
	ASpaceShooterGameState::OnGameEnded.AddUniqueDynamic(this, &ThisClass::OnGameEnded);
}

void ASpaceShooterHUD::OnGameStarted()
{
	if (ensure(GameplayScreen != nullptr))
	{
		GameplayScreen->AddToViewport(0);
	}
}

void ASpaceShooterHUD::OnGameEnded()
{
	if (ensure(GameplayScreen != nullptr))
	{
		GameplayScreen->RemoveFromParent();
	}
}
