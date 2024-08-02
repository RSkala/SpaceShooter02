// Copyright 2024 Richard Skala

#include "UI/SpaceShooterHUD.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "UI/GameplayScreen.h"

void ASpaceShooterHUD::BeginPlay()
{
	Super::BeginPlay();

	if (ensure(GameplayScreenClass != nullptr))
	{
		if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
		{
			GameplayScreen = Cast<UGameplayScreen>(UWidgetBlueprintLibrary::Create(GetWorld(), GameplayScreenClass, PlayerController));
			if (ensure(GameplayScreen != nullptr))
			{
				GameplayScreen->AddToViewport(0);
			}
		}
	}

	// TODO: Listen for GameState events:
	// * Main Menu:  Disable GameplayScreen
	// * Ship Select: Disable GameplayScreen
	// * Game Over: Disable GameplayScreen
	// * Game Started: Show GameplayScreen
}
