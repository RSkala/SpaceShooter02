// Copyright 2024 Richard Skala

#include "UI/GameOverScreen.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetTextLibrary.h"

#include "SpaceShooterGameState.h"
#include "UI/SpaceShooterMenuController.h"

#define LOCTEXT_NAMESPACE "GameOverScreen"

void UGameOverScreen::InitGameOverScreen(int32 FinalScore)
{
	if (FinalScoreText != nullptr)
	{
		//FText ScoreText = FText::FromString(FString::Printf(TEXT("FINAL SCORE: %d"), (int32)FinalScore));
		FText FinalScoreTextGrouped = UKismetTextLibrary::Conv_IntToText(FinalScore, false, true);
		static const FText FinalScoreTextFormat = LOCTEXT("FinalScoreText", "FINAL SCORE: {0}");
		FText ScoreText = FText::Format(FinalScoreTextFormat, FinalScoreTextGrouped);
		FinalScoreText->SetText(ScoreText);
	}
}

void UGameOverScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (PlayAgainButton != nullptr)
	{
		PlayAgainButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnPlayAgainButtonClicked);
		PlayAgainButton->SetNavigationRuleExplicit(EUINavigation::Right, QuitGameButton);
		PlayAgainButton->SetNavigationRuleExplicit(EUINavigation::Left, SelectNewShipButton);
	}

	if (SelectNewShipButton != nullptr)
	{
		SelectNewShipButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnSelectNewShipButtonClicked);
		SelectNewShipButton->SetNavigationRuleExplicit(EUINavigation::Right, PlayAgainButton);
	}

	if (QuitGameButton != nullptr)
	{
		QuitGameButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnQuitGameButtonClicked);
		QuitGameButton->SetNavigationRuleExplicit(EUINavigation::Left, PlayAgainButton);
	}
}

void UGameOverScreen::NativeConstruct()
{
	Super::NativeConstruct();

	if (PlayAgainButton != nullptr)
	{
		PlayAgainButton->SetKeyboardFocus();
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

	SetColorShiftForButton(PlayAgainButton, LinearColor);
	SetColorShiftForButton(SelectNewShipButton, LinearColor);
	SetColorShiftForButton(QuitGameButton, LinearColor);
}

void UGameOverScreen::OnPlayAgainButtonClicked()
{
	UE_LOG(LogMenus, Log, TEXT("UGameOverScreen::OnPlayAgainButtonClicked"));
	USpaceShooterMenuController::OnGameOverPlayAgainClicked.Broadcast();
}

void UGameOverScreen::OnSelectNewShipButtonClicked()
{
	UE_LOG(LogMenus, Log, TEXT("UGameOverScreen::OnSelectNewShipButtonClicked"));
	USpaceShooterMenuController::OnGameOverSelectShipClicked.Broadcast();
}

void UGameOverScreen::OnQuitGameButtonClicked()
{
	UE_LOG(LogMenus, Log, TEXT("Quitting game from Game Over Screen..."));
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

#undef LOCTEXT_NAMESPACE
