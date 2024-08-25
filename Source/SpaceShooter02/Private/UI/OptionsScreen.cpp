// Copyright 2024 Richard Skala

#include "UI/OptionsScreen.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

#include "AudioEnums.h"
#include "SpaceShooterGameInstance.h"
#include "UI/SpaceShooterMenuController.h"

void UOptionsScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (HowToPlayButton != nullptr)
	{
		HowToPlayButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnHowToPlayButtonClicked);
		HowToPlayButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnHowToPlayButtonHovered);
		HowToPlayButton->SetNavigationRuleExplicit(EUINavigation::Up, BackButton);
		HowToPlayButton->SetNavigationRuleExplicit(EUINavigation::Down, CreditsButton);
	}

	if (CreditsButton != nullptr)
	{
		CreditsButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnCreditsButtonClicked);
		CreditsButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnCreditsButtonHovered);
		CreditsButton->SetNavigationRuleExplicit(EUINavigation::Up, HowToPlayButton);
		CreditsButton->SetNavigationRuleExplicit(EUINavigation::Down, StatsButton);
	}

	if (StatsButton != nullptr)
	{
		StatsButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnStatsButtonClicked);
		StatsButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnStatsButtonHovered);
		StatsButton->SetNavigationRuleExplicit(EUINavigation::Up, CreditsButton);
		StatsButton->SetNavigationRuleExplicit(EUINavigation::Down, SoundsButton);
	}

	if (SoundsButton != nullptr)
	{
		SoundsButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnSoundsButtonClicked);
		SoundsButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnSoundsButtonHovered);
		SoundsButton->SetNavigationRuleExplicit(EUINavigation::Up, StatsButton);
		SoundsButton->SetNavigationRuleExplicit(EUINavigation::Down, DataButton);
	}

	if (DataButton != nullptr)
	{
		DataButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnDataButtonClicked);
		DataButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnDataButtonHovered);
		DataButton->SetNavigationRuleExplicit(EUINavigation::Up, SoundsButton);
		DataButton->SetNavigationRuleExplicit(EUINavigation::Down, BackButton);
	}

	if (BackButton != nullptr)
	{
		BackButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnBackButtonClicked);
		BackButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnBackButtonHovered);
		BackButton->SetNavigationRuleExplicit(EUINavigation::Up, DataButton);
		BackButton->SetNavigationRuleExplicit(EUINavigation::Down, HowToPlayButton);
	}
}

void UOptionsScreen::OnColorShift(FLinearColor LinearColor)
{
	Super::OnColorShift(LinearColor);

	if (OptionsTextBlock != nullptr)
	{
		OptionsTextBlock->SetColorAndOpacity(FSlateColor(LinearColor));
	}

	SetColorShiftForButton(HowToPlayButton, LinearColor);
	SetColorShiftForButton(CreditsButton, LinearColor);
	SetColorShiftForButton(SoundsButton, LinearColor);
	SetColorShiftForButton(StatsButton, LinearColor);
	SetColorShiftForButton(DataButton, LinearColor);
	SetColorShiftForButton(BackButton, LinearColor);
}

void UOptionsScreen::OnHowToPlayButtonClicked()
{
	USpaceShooterMenuController::OnOptionsScreenHowToPlayClicked.Broadcast();
}

void UOptionsScreen::OnCreditsButtonClicked()
{
	USpaceShooterMenuController::OnOptionsScreenCreditsClicked.Broadcast();
}

void UOptionsScreen::OnStatsButtonClicked()
{
	USpaceShooterMenuController::OnOptionsScreenStatsClicked.Broadcast();
}

void UOptionsScreen::OnSoundsButtonClicked()
{
	USpaceShooterMenuController::OnOptionsScreenSoundsClicked.Broadcast();
}

void UOptionsScreen::OnDataButtonClicked()
{
	USpaceShooterMenuController::OnOptionsScreenDataClicked.Broadcast();
}

void UOptionsScreen::OnBackButtonClicked()
{
	USpaceShooterMenuController::OnOptionsScreenBackClicked.Broadcast();
}

void UOptionsScreen::OnHowToPlayButtonHovered()
{
	if (HowToPlayButton != nullptr)
	{
		HowToPlayButton->SetKeyboardFocus();
	}
}

void UOptionsScreen::OnCreditsButtonHovered()
{
	if (CreditsButton != nullptr)
	{
		CreditsButton->SetKeyboardFocus();
	}
}

void UOptionsScreen::OnStatsButtonHovered()
{
	if (StatsButton != nullptr)
	{
		StatsButton->SetKeyboardFocus();
	}
}

void UOptionsScreen::OnSoundsButtonHovered()
{
	if (SoundsButton != nullptr)
	{
		SoundsButton->SetKeyboardFocus();
	}
}

void UOptionsScreen::OnDataButtonHovered()
{
	if (DataButton != nullptr)
	{
		DataButton->SetKeyboardFocus();
	}
}

void UOptionsScreen::OnBackButtonHovered()
{
	if (BackButton != nullptr)
	{
		BackButton->SetKeyboardFocus();
	}
}
