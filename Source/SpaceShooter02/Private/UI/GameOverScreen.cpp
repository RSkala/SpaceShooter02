// Copyright 2024 Richard Skala

#include "UI/GameOverScreen.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"

#include "SpaceShooterGameState.h"

void UGameOverScreen::InitGameOverScreen(int32 FinalScore)
{
	if (FinalScoreText != nullptr)
	{
		FText ScoreText = FText::FromString(FString::Printf(TEXT("FINAL SCORE: %d"), (int32)FinalScore));
		FinalScoreText->SetText(ScoreText);
	}
}

void UGameOverScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (PlayAgainButton != nullptr)
	{
		PlayAgainButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnPlayAgainButtonClicked);
	}

	if (SelectNewShipButton != nullptr)
	{
		SelectNewShipButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnSelectNewShipButtonClicked);
	}

	if (QuitGameButton != nullptr)
	{
		QuitGameButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnQuitGameButtonClicked);
	}
}

void UGameOverScreen::OnColorShift(FLinearColor LinearColor)
{
	Super::OnColorShift(LinearColor);

	if (GameOverText != nullptr)
	{
		GameOverText->SetColorAndOpacity(LinearColor);
	}

	if (FinalScoreText != nullptr)
	{
		FinalScoreText->SetColorAndOpacity(LinearColor);
	}
}

void UGameOverScreen::OnPlayAgainButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("UGameOverScreen::OnPlayAgainButtonClicked"));
	if (UWorld* World = GetWorld())
	{
		if (ASpaceShooterGameState* SpaceShooterGameState = World->GetGameState<ASpaceShooterGameState>())
		{
			SpaceShooterGameState->StartGame();
		}
	}
}

void UGameOverScreen::OnSelectNewShipButtonClicked()
{
	UE_LOG(LogMenus, Warning, TEXT("UGameOverScreen::OnSelectNewShipButtonClicked"));
}

void UGameOverScreen::OnQuitGameButtonClicked()
{
	UE_LOG(LogMenus, Warning, TEXT("Quitting game from Game Over Screen..."));
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
