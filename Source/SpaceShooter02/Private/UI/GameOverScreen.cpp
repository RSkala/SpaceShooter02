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
		PlayAgainButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnPlayAgainButtonHovered);
		PlayAgainButton->SetNavigationRuleExplicit(EUINavigation::Down, SelectNewShipButton);
	}

	if (SelectNewShipButton != nullptr)
	{
		SelectNewShipButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnSelectNewShipButtonClicked);
		SelectNewShipButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnSelectNewShipButtonHovered);
		SelectNewShipButton->SetNavigationRuleExplicit(EUINavigation::Up, PlayAgainButton);
		SelectNewShipButton->SetNavigationRuleExplicit(EUINavigation::Down, QuitGameButton);
	}

	if (QuitGameButton != nullptr)
	{
		QuitGameButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnQuitGameButtonClicked);
		QuitGameButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnQuitGameButtonHovered);
		QuitGameButton->SetNavigationRuleExplicit(EUINavigation::Up, SelectNewShipButton);
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
	USpaceShooterMenuController::OnGameOverMainMenuClicked.Broadcast();
}

void UGameOverScreen::OnPlayAgainButtonHovered()
{
	if (PlayAgainButton != nullptr)
	{
		PlayAgainButton->SetKeyboardFocus();
	}
}

void UGameOverScreen::OnSelectNewShipButtonHovered()
{
	if (SelectNewShipButton != nullptr)
	{
		SelectNewShipButton->SetKeyboardFocus();
	}
}

void UGameOverScreen::OnQuitGameButtonHovered()
{
	if (QuitGameButton != nullptr)
	{
		QuitGameButton->SetKeyboardFocus();
	}
}

#undef LOCTEXT_NAMESPACE
