// Copyright 2024 Richard Skala

#include "UI/GameplayScreen.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"

#include "PlayerShipPawn.h"
#include "SpaceShooterGameState.h"

#define LOCTEXT_NAMESPACE "GameplayScreen"

void UGameplayScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Bind to game state delegate callbacks
	ASpaceShooterGameState::OnPlayerScoreChanged.AddUniqueDynamic(this, &ThisClass::OnPlayerScoreUpdated);
	ASpaceShooterGameState::OnPlayerMultiplierChanged.AddUniqueDynamic(this, &ThisClass::OnPlayerScoreMultiplierUpdated);
	ASpaceShooterGameState::OnPlayerHighScoreChanged.AddUniqueDynamic(this, &ThisClass::OnPlayerHighScoreUpdated);

	// Bind to player delegate callbacks
	APlayerShipPawn::OnPlayerPowerupTimerUpdated.AddUniqueDynamic(this, &ThisClass::OnPowerupTimeUpdated);

	// Force powerup meter empty
	OnPowerupTimeUpdated(0.0f);
}

void UGameplayScreen::OnPlayerScoreUpdated(int32 PlayerScore)
{
	UE_LOG(LogTemp, Warning, TEXT("UGameplayScreen::OnPlayerScoreUpdated"));

	if (CurrentScoreText != nullptr)
	{
		// Convert the score to Text using Grouping (i.e. comma for thousands separators, depending on locale)
		FText ScoreText = UKismetTextLibrary::Conv_IntToText(PlayerScore, false, true);
		//FText ScoreText = FText::FromString(FString::Printf(TEXT("%d"), PlayerScore));
		CurrentScoreText->SetText(ScoreText);
	}
}

void UGameplayScreen::OnPlayerScoreMultiplierUpdated(int32 PlayerScoreMultiplier)
{
	UE_LOG(LogTemp, Warning, TEXT("UGameplayScreen::OnPlayerScoreMultiplierUpdated"));

	if (CurrentMultiplierText != nullptr)
	{
		FText MultiplierTextGrouped = UKismetTextLibrary::Conv_IntToText(PlayerScoreMultiplier, false, true);
		static const FText MultiplierTextFormat = LOCTEXT("MultiplierText", "x{0}");
		FText ScoreMultiplierText = FText::Format(MultiplierTextFormat, MultiplierTextGrouped);
		CurrentMultiplierText->SetText(ScoreMultiplierText);
	}
}

void UGameplayScreen::OnPlayerHighScoreUpdated(int32 PlayerHighScore)
{
	UE_LOG(LogTemp, Warning, TEXT("UGameplayScreen::OnPlayerHighScoreUpdated"));

	if (HighScoreText != nullptr)
	{
		//FText ScoreText = FText::FromString(FString::Printf(TEXT("BEST: %d"), PlayerHighScore));
		FText HighScoreTextGrouped = UKismetTextLibrary::Conv_IntToText(PlayerHighScore, false, true);
		static const FText HighScoreTextFormat = LOCTEXT("HighScoreText", "BEST: {0}");
		FText ScoreText = FText::Format(HighScoreTextFormat, HighScoreTextGrouped);
		HighScoreText->SetText(ScoreText);
	}
}

void UGameplayScreen::OnPowerupTimeUpdated(float Percent)
{
	if (PowerupWeaponMeter != nullptr)
	{
		PowerupWeaponMeter->SetPercent(Percent);

		/*if (Percent <= 0.0f)
		{
			PowerupWeaponMeter->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			PowerupWeaponMeter->SetVisibility(ESlateVisibility::HitTestInvisible);
		}*/
	}
}

#undef LOCTEXT_NAMESPACE
