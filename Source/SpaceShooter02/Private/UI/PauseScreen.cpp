// Copyright 2024 Richard Skala

#include "UI/PauseScreen.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "SpaceShooterGameState.h"

void UPauseScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ResumeButton != nullptr)
	{
		ResumeButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnResumeButtonClicked);
		ResumeButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnResumeButtonHovered);
		ResumeButton->SetNavigationRuleExplicit(EUINavigation::Down, QuitGameButton);
	}

	if (QuitGameButton != nullptr)
	{
		QuitGameButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnQuitButtonClicked);
		QuitGameButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnQuitButtonHovered);
		QuitGameButton->SetNavigationRuleExplicit(EUINavigation::Up, ResumeButton);
	}
}

void UPauseScreen::OnColorShift(FLinearColor ShiftColor)
{
	Super::OnColorShift(ShiftColor);

	if (PausedTextBlock != nullptr)
	{
		PausedTextBlock->SetColorAndOpacity(FSlateColor(ShiftColor));
	}

	SetColorShiftForButton(ResumeButton, ShiftColor);
	SetColorShiftForButton(QuitGameButton, ShiftColor);
}

void UPauseScreen::OnResumeButtonClicked()
{
	ASpaceShooterGameState::OnRequestUnpauseGame.Broadcast();
}

void UPauseScreen::OnResumeButtonHovered()
{
	if (ResumeButton != nullptr)
	{
		ResumeButton->SetKeyboardFocus();
	}
}

void UPauseScreen::OnQuitButtonClicked()
{
	ASpaceShooterGameState::OnRequestSelfDestruct.ExecuteIfBound();
}

void UPauseScreen::OnQuitButtonHovered()
{
	if (QuitGameButton != nullptr)
	{
		QuitGameButton->SetKeyboardFocus();
	}
}
