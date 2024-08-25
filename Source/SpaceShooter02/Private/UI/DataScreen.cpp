// Copyright 2024 Richard Skala

#include "UI/DataScreen.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "UI/SpaceShooterMenuController.h"

void UDataScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ClearScoresButton != nullptr)
	{
		ClearScoresButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnClearScoresButtonClicked);
		ClearScoresButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnClearScoresButtonHovered);
		ClearScoresButton->SetNavigationRuleExplicit(EUINavigation::Down, ClearStatsButton);
	}

	if (ClearStatsButton != nullptr)
	{
		ClearStatsButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnClearStatsButtonClicked);
		ClearStatsButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnClearStatsButtonHovered);
		ClearStatsButton->SetNavigationRuleExplicit(EUINavigation::Up, ClearScoresButton);
		ClearStatsButton->SetNavigationRuleExplicit(EUINavigation::Down, BackButton);
	}

	if (BackButton != nullptr)
	{
		BackButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnBackButtonClicked);
		BackButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnBackButtonHovered);
		BackButton->SetNavigationRuleExplicit(EUINavigation::Up, ClearStatsButton);
	}
}

void UDataScreen::OnColorShift(FLinearColor LinearColor)
{
	Super::OnColorShift(LinearColor);

	if (DataTextBlock != nullptr)
	{
		DataTextBlock->SetColorAndOpacity(FSlateColor(LinearColor));
	}

	SetColorShiftForButton(ClearScoresButton, LinearColor);
	SetColorShiftForButton(ClearStatsButton, LinearColor);
	SetColorShiftForButton(BackButton, LinearColor);
}

void UDataScreen::OnClearScoresButtonClicked()
{
	USpaceShooterMenuController::OnDataScreenClearScoresClicked.Broadcast();
}

void UDataScreen::OnClearStatsButtonClicked()
{
	USpaceShooterMenuController::OnDataScreenClearStatsClicked.Broadcast();
}

void UDataScreen::OnBackButtonClicked()
{
	USpaceShooterMenuController::OnDataScreenBackClicked.Broadcast();
}

void UDataScreen::OnClearScoresButtonHovered()
{
	if (ClearScoresButton != nullptr)
	{
		ClearScoresButton->SetKeyboardFocus();
	}
}

void UDataScreen::OnClearStatsButtonHovered()
{
	if (ClearStatsButton != nullptr)
	{
		ClearStatsButton->SetKeyboardFocus();
	}
}

void UDataScreen::OnBackButtonHovered()
{
	if (BackButton != nullptr)
	{
		BackButton->SetKeyboardFocus();
	}
}
