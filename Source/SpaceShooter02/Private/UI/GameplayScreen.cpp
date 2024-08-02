// Copyright 2024 Richard Skala

#include "UI/GameplayScreen.h"

#include "Components/TextBlock.h"

#include "SpaceShooterGameState.h"

void UGameplayScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Bind to game state delegate callbacks
	ASpaceShooterGameState::OnPlayerScoreChanged.AddUniqueDynamic(this, &ThisClass::OnPlayerScoreUpdated);
	ASpaceShooterGameState::OnPlayerMultiplierChanged.AddUniqueDynamic(this, &ThisClass::OnPlayerScoreMultiplierUpdated);
	ASpaceShooterGameState::OnPlayerHighScoreChanged.AddUniqueDynamic(this, &ThisClass::OnPlayerHighScoreUpdated);
}

void UGameplayScreen::OnPlayerScoreUpdated(int32 PlayerScore)
{
	UE_LOG(LogTemp, Warning, TEXT("UGameplayScreen::OnPlayerScoreUpdated"));

	if (CurrentScoreText != nullptr)
	{
		FText ScoreText = FText::FromString(FString::Printf(TEXT("%d"), PlayerScore));
		CurrentScoreText->SetText(ScoreText);
	}
}

void UGameplayScreen::OnPlayerScoreMultiplierUpdated(int32 PlayerScoreMultiplier)
{
	UE_LOG(LogTemp, Warning, TEXT("UGameplayScreen::OnPlayerScoreMultiplierUpdated"));

	if (CurrentMultiplierText != nullptr)
	{
		FText ScoreMultiplierText = FText::FromString(FString::Printf(TEXT("x%d"), PlayerScoreMultiplier));
		CurrentMultiplierText->SetText(ScoreMultiplierText);
	}
}

void UGameplayScreen::OnPlayerHighScoreUpdated(int32 PlayerHighScore)
{
	UE_LOG(LogTemp, Warning, TEXT("UGameplayScreen::OnPlayerHighScoreUpdated"));

	if (HighScoreText != nullptr)
	{
		FText ScoreText = FText::FromString(FString::Printf(TEXT("BEST: %d"), PlayerHighScore));
		HighScoreText->SetText(ScoreText);
	}
}
