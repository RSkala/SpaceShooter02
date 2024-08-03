// Copyright 2024 Richard Skala

#include "UI/GameCreditsScreen.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "UI/SpaceShooterMenuController.h"

// NOTE: These do not update when using Live Coding
namespace GameCreditsScreen
{
	static const FString CreditsLine1 = "PROGRAMMING & DESIGN\nRichard Skala\nrskala.github.io\n\n\n";
	static const FString CreditsLine2 = "ART\nGustavo Vituri\ngvituri.itch.io\n\n\n";
	static const FString CreditsLine3 = "SOUND EFFECTS\nSubspaceAudio\nsubspaceaudio.itch.io\n\n\n";
	static const FString CreditsLine4 = "PIXEL CROSSHAIRS\nDonut Studio\ndonut-studio.itch.io\n\n\n";
	static const FString CreditsLine5 = "\n\nBuilt in Unreal Engine 5.4\n\n";
}

void UGameCreditsScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (CreditsTextBlock != nullptr)
	{
		FText CreditsText = FText::FromString(
			GameCreditsScreen::CreditsLine1 +
			GameCreditsScreen::CreditsLine2 +
			GameCreditsScreen::CreditsLine3 +
			GameCreditsScreen::CreditsLine4 +
			GameCreditsScreen::CreditsLine5);
		CreditsTextBlock->SetText(CreditsText);
	}

	if (BackButton != nullptr)
	{
		BackButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnBackButtonClicked);
	}
}

void UGameCreditsScreen::OnColorShift(FLinearColor LinearColor)
{
	Super::OnColorShift(LinearColor);
	SetColorShiftForButton(BackButton, LinearColor);
}

void UGameCreditsScreen::OnBackButtonClicked()
{
	USpaceShooterMenuController::OnCreditsMenuBackClicked.Broadcast();
}
