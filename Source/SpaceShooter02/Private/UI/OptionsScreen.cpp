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
		CreditsButton->SetNavigationRuleExplicit(EUINavigation::Down, BackButton);
	}

	if (BackButton != nullptr)
	{
		BackButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnBackButtonClicked);
		BackButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnBackButtonHovered);
		BackButton->SetNavigationRuleExplicit(EUINavigation::Up, CreditsButton);
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
}

void UOptionsScreen::OnCreditsButtonClicked()
{
	USpaceShooterMenuController::OnOptionsScreenCreditsClicked.Broadcast();
}

void UOptionsScreen::OnBackButtonClicked()
{
	USpaceShooterMenuController::OnOptionsScreenBackClicked.Broadcast();
}

void UOptionsScreen::OnCreditsButtonHovered()
{
	if (CreditsButton != nullptr)
	{
		CreditsButton->SetKeyboardFocus();
	}
}

void UOptionsScreen::OnBackButtonHovered()
{
	if (BackButton != nullptr)
	{
		BackButton->SetKeyboardFocus();
	}
}
