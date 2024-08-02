// Copyright 2024 Richard Skala

#include "UI/PlayerShipSelectScreen.h"

#include "Components/Button.h"

#include "SpaceShooterGameState.h"
#include "UI/SpaceShooterMenuController.h"

void UPlayerShipSelectScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (LaunchButton != nullptr)
	{
		LaunchButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnLaunchButtonClicked);
	}
}

void UPlayerShipSelectScreen::OnLaunchButtonClicked()
{
	USpaceShooterMenuController::OnPlayerShipSelected.Broadcast();
}
