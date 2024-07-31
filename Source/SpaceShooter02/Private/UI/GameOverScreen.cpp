// Copyright 2024 Richard Skala

#include "UI/GameOverScreen.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"

void UGameOverScreen::InitGameOverScreen(int32 FinalScore)
{
	if (FinalScoreText != nullptr)
	{
		FText ScoreText = FText::FromString(FString::Printf(TEXT("%d"), (int32)FinalScore));
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
}

void UGameOverScreen::OnPlayAgainButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("UGameOverScreen::OnPlayAgainButtonClicked"));
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
