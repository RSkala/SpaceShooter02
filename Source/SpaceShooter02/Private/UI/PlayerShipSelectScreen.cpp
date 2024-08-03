// Copyright 2024 Richard Skala

#include "UI/PlayerShipSelectScreen.h"

#include "Components/Button.h"

#include "PaperSprite.h"

#include "SpaceShooterGameState.h"
#include "UI/SpaceShooterMenuController.h"

void UPlayerShipSelectScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (LaunchButton != nullptr)
	{
		LaunchButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnLaunchButtonClicked);
	}

	if (ShipSelectButton1 != nullptr)
	{
		ShipSelectButton1->OnClicked.AddUniqueDynamic(this, &ThisClass::OnShipSelectButton1Clicked);
	}

	if (ShipSelectButton2 != nullptr)
	{
		ShipSelectButton2->OnClicked.AddUniqueDynamic(this, &ThisClass::OnShipSelectButton2Clicked);
	}

	if (ShipSelectButton3 != nullptr)
	{
		ShipSelectButton3->OnClicked.AddUniqueDynamic(this, &ThisClass::OnShipSelectButton3Clicked);
	}

	if (ShipSelectButton4 != nullptr)
	{
		ShipSelectButton4->OnClicked.AddUniqueDynamic(this, &ThisClass::OnShipSelectButton4Clicked);
	}

	if (ShipSelectButton5 != nullptr)
	{
		ShipSelectButton5->OnClicked.AddUniqueDynamic(this, &ThisClass::OnShipSelectButton5Clicked);
	}
}

void UPlayerShipSelectScreen::OnLaunchButtonClicked()
{
	USpaceShooterMenuController::OnPlayerShipSelected.Broadcast(nullptr); // RKS: To be removed
}

void UPlayerShipSelectScreen::OnShipSelectButton1Clicked()
{
	USpaceShooterMenuController::OnPlayerShipSelected.Broadcast(ShipSprite1);
}

void UPlayerShipSelectScreen::OnShipSelectButton2Clicked()
{
	USpaceShooterMenuController::OnPlayerShipSelected.Broadcast(ShipSprite2);
}

void UPlayerShipSelectScreen::OnShipSelectButton3Clicked()
{
	USpaceShooterMenuController::OnPlayerShipSelected.Broadcast(ShipSprite3);
}

void UPlayerShipSelectScreen::OnShipSelectButton4Clicked()
{
	USpaceShooterMenuController::OnPlayerShipSelected.Broadcast(ShipSprite4);
}

void UPlayerShipSelectScreen::OnShipSelectButton5Clicked()
{
	USpaceShooterMenuController::OnPlayerShipSelected.Broadcast(ShipSprite5);
}
