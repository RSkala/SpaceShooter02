// Copyright 2024 Richard Skala

#include "UI/HowToPlayScreen.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "UI/SpaceShooterMenuController.h"

void UHowToPlayScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (BackButton != nullptr)
	{
		BackButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnBackButtonClicked);
		BackButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnBackButtonHovered);
	}

	FString TopSpace = TEXT("\n\n\n");
	HowToPlayLine1 = TEXT("* MOVE w/ Left Thumbstick or WASD.\n\n");
	HowToPlayLine2 = TEXT("* AIM + FIRE w/ Right Thumbstick or Mouse.\n\n");
	HowToPlayLine3 = TEXT("* BOOST w/ shoulder buttons or Space.\n\n");
	HowToPlayLine4 = TEXT("* BOOST refills every 5 seconds.\n\n");
	HowToPlayLine5 = TEXT("* Ship is invincible during BOOST!\n\n");
	HowToPlayLine6 = TEXT("* Enemies drop SCORE MULTIPLIERS.\n\n");
	HowToPlayLine7 = TEXT("* Pick up 8 SCORE MULTIPLIERS to POWER UP!\n");

	if (HowToPlayTextBlock != nullptr)
	{
		FText HowToPlayText = FText::FromString(
			TopSpace +
			HowToPlayLine1 +
			HowToPlayLine2 +
			HowToPlayLine3 +
			HowToPlayLine4 +
			HowToPlayLine5 +
			HowToPlayLine6 +
			HowToPlayLine7);

		HowToPlayTextBlock->SetText(HowToPlayText);
	}
}

void UHowToPlayScreen::OnColorShift(FLinearColor LinearColor)
{
	Super::OnColorShift(LinearColor);

	if (HowToPlayTitleTextBlock != nullptr)
	{
		HowToPlayTitleTextBlock->SetColorAndOpacity(FSlateColor(LinearColor));
	}

	if (HowToPlayTextBlock != nullptr)
	{
		HowToPlayTextBlock->SetColorAndOpacity(FSlateColor(LinearColor));
	}

	SetColorShiftForButton(BackButton, LinearColor);
}

void UHowToPlayScreen::OnBackButtonClicked()
{
	USpaceShooterMenuController::OnHowToPlayScreenBackClicked.Broadcast();
}

void UHowToPlayScreen::OnBackButtonHovered()
{
	if (BackButton != nullptr)
	{
		BackButton->SetKeyboardFocus();
	}
}
