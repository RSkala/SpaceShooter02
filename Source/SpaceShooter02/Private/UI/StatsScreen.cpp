// Copyright 2024 Richard Skala

#include "UI/StatsScreen.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "UI/SpaceShooterMenuController.h"

void UStatsScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (BackButton != nullptr)
	{
		BackButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnBackButtonClicked);
	}
}

void UStatsScreen::OnColorShift(FLinearColor LinearColor)
{
	Super::OnColorShift(LinearColor);
	SetColorShiftForButton(BackButton, LinearColor);

	if (StatsTitleTextBlock != nullptr)
	{
		StatsTitleTextBlock->SetColorAndOpacity(LinearColor);
	}
}

void UStatsScreen::OnBackButtonClicked()
{
	USpaceShooterMenuController::OnStatsScreenBackClicked.Broadcast();
}

void UStatsScreen::OnBackButtonHovered()
{
	if (BackButton != nullptr)
	{
		BackButton->SetKeyboardFocus();
	}
}
