// Copyright 2024 Richard Skala

#include "UI/PlayerShipSelectScreen.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "PaperSprite.h"

#include "SpaceShooterGameState.h"
#include "UI/ShipSelectionWidget.h"
#include "UI/SpaceShooterMenuController.h"

void UPlayerShipSelectScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (BackButton != nullptr)
	{
		BackButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnBackButtonClicked);
	}

	// -------------------------------------------------
	if (ShipSelectionWidget1 != nullptr)
	{
		ShipSelectionWidget1->InitShipSelectionWidget(
			TEXT("SHIP 01"),
			TEXT("WHITE"),
			FLinearColor::White,
			ShipSprites.Num() > 0 ? ShipSprites[0] : nullptr);
	}

	if (ShipSelectionWidget2 != nullptr)
	{
		ShipSelectionWidget2->InitShipSelectionWidget(
			TEXT("SHIP 02"),
			TEXT("ORANGE"),
			FLinearColor(1.0f, 0.262251f, 0.0f),
			ShipSprites.Num() > 1 ? ShipSprites[1] : nullptr);
	}

	if (ShipSelectionWidget3 != nullptr)
	{
		ShipSelectionWidget3->InitShipSelectionWidget(
			TEXT("SHIP 03"),
			TEXT("GREEN"),
			FLinearColor(0.165132f, 0.768151f, 0.287441f),
			ShipSprites.Num() > 2 ? ShipSprites[2] : nullptr);
	}

	if (ShipSelectionWidget4 != nullptr)
	{
		ShipSelectionWidget4->InitShipSelectionWidget(
			TEXT("SHIP 04"),
			TEXT("PURPLE"),
			FLinearColor(0.361307f, 0.165132f, 1.0f),
			ShipSprites.Num() > 3 ? ShipSprites[3] : nullptr);
	}

	if (ShipSelectionWidget5 != nullptr)
	{
		ShipSelectionWidget5->InitShipSelectionWidget(
			TEXT("SHIP 05"),
			TEXT("RED"),
			FLinearColor(0.445201f, 0.005182f, 0.029557f),
			ShipSprites.Num() > 4 ? ShipSprites[4] : nullptr);
	}
}

void UPlayerShipSelectScreen::NativeConstruct()
{
	Super::NativeConstruct();

	
	BackButton->SetNavigationRuleExplicit(EUINavigation::Left, ShipSelectionWidget5->GetLaunchButton());
	BackButton->SetNavigationRuleExplicit(EUINavigation::Right, ShipSelectionWidget1->GetLaunchButton());

	if (ShipSelectionWidget1 != nullptr &&
		ShipSelectionWidget2 != nullptr &&
		ShipSelectionWidget3 != nullptr &&
		ShipSelectionWidget4 != nullptr &&
		ShipSelectionWidget5 != nullptr)
	{
		ShipSelectionWidget1->GetLaunchButton()->SetNavigationRuleExplicit(EUINavigation::Left, BackButton);
		ShipSelectionWidget1->GetLaunchButton()->SetNavigationRuleExplicit(EUINavigation::Right, ShipSelectionWidget2->GetLaunchButton());
		ShipSelectionWidget1->GetLaunchButton()->SetNavigationRuleExplicit(EUINavigation::Down, BackButton);

		ShipSelectionWidget2->GetLaunchButton()->SetNavigationRuleExplicit(EUINavigation::Left, ShipSelectionWidget1->GetLaunchButton());
		ShipSelectionWidget2->GetLaunchButton()->SetNavigationRuleExplicit(EUINavigation::Right, ShipSelectionWidget3->GetLaunchButton());
		ShipSelectionWidget2->GetLaunchButton()->SetNavigationRuleExplicit(EUINavigation::Down, BackButton);

		ShipSelectionWidget3->GetLaunchButton()->SetNavigationRuleExplicit(EUINavigation::Left, ShipSelectionWidget2->GetLaunchButton());
		ShipSelectionWidget3->GetLaunchButton()->SetNavigationRuleExplicit(EUINavigation::Right, ShipSelectionWidget4->GetLaunchButton());
		ShipSelectionWidget3->GetLaunchButton()->SetNavigationRuleExplicit(EUINavigation::Down, BackButton);

		ShipSelectionWidget4->GetLaunchButton()->SetNavigationRuleExplicit(EUINavigation::Left, ShipSelectionWidget3->GetLaunchButton());
		ShipSelectionWidget4->GetLaunchButton()->SetNavigationRuleExplicit(EUINavigation::Right, ShipSelectionWidget5->GetLaunchButton());
		ShipSelectionWidget4->GetLaunchButton()->SetNavigationRuleExplicit(EUINavigation::Down, BackButton);

		ShipSelectionWidget5->GetLaunchButton()->SetNavigationRuleExplicit(EUINavigation::Left, ShipSelectionWidget4->GetLaunchButton());
		ShipSelectionWidget5->GetLaunchButton()->SetNavigationRuleExplicit(EUINavigation::Right, BackButton);
		ShipSelectionWidget5->GetLaunchButton()->SetNavigationRuleExplicit(EUINavigation::Down, BackButton);
	}
	
	if (BackButton != nullptr)
	{
		BackButton->SetKeyboardFocus();
	}
}

void UPlayerShipSelectScreen::OnColorShift(FLinearColor LinearColor)
{
	Super::OnColorShift(LinearColor);

	SetColorShiftForButton(BackButton, LinearColor);
	if (SelectYourShipTextBlock != nullptr)
	{
		SelectYourShipTextBlock->SetColorAndOpacity(FSlateColor(LinearColor));
	}

	if (ShipSelectionWidget1 != nullptr)
	{
		SetColorShiftForButton(ShipSelectionWidget1->GetLaunchButton(), LinearColor);
	}

	if (ShipSelectionWidget2 != nullptr)
	{
		SetColorShiftForButton(ShipSelectionWidget2->GetLaunchButton(), LinearColor);
	}

	if (ShipSelectionWidget3 != nullptr)
	{
		SetColorShiftForButton(ShipSelectionWidget3->GetLaunchButton(), LinearColor);
	}

	if (ShipSelectionWidget4 != nullptr)
	{
		SetColorShiftForButton(ShipSelectionWidget4->GetLaunchButton(), LinearColor);
	}

	if (ShipSelectionWidget5 != nullptr)
	{
		SetColorShiftForButton(ShipSelectionWidget5->GetLaunchButton(), LinearColor);
	}
}

void UPlayerShipSelectScreen::OnBackButtonClicked()
{
	USpaceShooterMenuController::OnShipSelectBackClicked.Broadcast();
}
