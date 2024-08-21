// Copyright 2024 Richard Skala

#include "UI/OptionsScreen.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "UI/SpaceShooterMenuController.h"

void UOptionsScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (CreditsButton != nullptr)
	{
		CreditsButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnCreditsButtonClicked);
		CreditsButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnCreditsButtonHovered);
		CreditsButton->SetNavigationRuleExplicit(EUINavigation::Down, StatsButton);
	}

	if (StatsButton != nullptr)
	{
		StatsButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnStatsButtonClicked);
		StatsButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnStatsButtonHovered);
		StatsButton->SetNavigationRuleExplicit(EUINavigation::Up, CreditsButton);
		StatsButton->SetNavigationRuleExplicit(EUINavigation::Down, ClearScoresButton);
	}

	if (ClearScoresButton != nullptr)
	{
		ClearScoresButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnClearScoresButtonClicked);
		ClearScoresButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnClearScoresButtonHovered);
		ClearScoresButton->SetNavigationRuleExplicit(EUINavigation::Up, StatsButton);
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

void UOptionsScreen::OnColorShift(FLinearColor LinearColor)
{
	Super::OnColorShift(LinearColor);

	if (OptionsTextBlock != nullptr)
	{
		OptionsTextBlock->SetColorAndOpacity(FSlateColor(LinearColor));
	}

	SetColorShiftForButton(CreditsButton, LinearColor);
	SetColorShiftForButton(BackButton, LinearColor);
	SetColorShiftForButton(ClearScoresButton, LinearColor);
	SetColorShiftForButton(StatsButton, LinearColor);
	SetColorShiftForButton(ClearStatsButton, LinearColor);
}

void UOptionsScreen::OnCreditsButtonClicked()
{
	USpaceShooterMenuController::OnOptionsScreenCreditsClicked.Broadcast();
}

void UOptionsScreen::OnBackButtonClicked()
{
	USpaceShooterMenuController::OnOptionsScreenBackClicked.Broadcast();
}

void UOptionsScreen::OnClearScoresButtonClicked()
{
	USpaceShooterMenuController::OnOptionsScreenClearScoresClicked.Broadcast();
}

void UOptionsScreen::OnStatsButtonClicked()
{
	USpaceShooterMenuController::OnOptionsScreenStatsClicked.Broadcast();
}

void UOptionsScreen::OnClearStatsButtonClicked()
{
	USpaceShooterMenuController::OnOptionsScreenClearStatsClicked.Broadcast();
}

void UOptionsScreen::OnCreditsButtonHovered()
{
	if (CreditsButton != nullptr)
	{
		CreditsButton->SetKeyboardFocus();
	}
}

void UOptionsScreen::OnClearScoresButtonHovered()
{
	if (ClearScoresButton != nullptr)
	{
		ClearScoresButton->SetKeyboardFocus();
	}
}

void UOptionsScreen::OnBackButtonHovered()
{
	if (BackButton != nullptr)
	{
		BackButton->SetKeyboardFocus();
	}
}

void UOptionsScreen::OnStatsButtonHovered()
{
	if (StatsButton != nullptr)
	{
		StatsButton->SetKeyboardFocus();
	}
}

void UOptionsScreen::OnClearStatsButtonHovered()
{
	if (ClearStatsButton != nullptr)
	{
		ClearStatsButton->SetKeyboardFocus();
	}
}
