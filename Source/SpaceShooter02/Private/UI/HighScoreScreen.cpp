// Copyright 2024 Richard Skala

#include "UI/HighScoreScreen.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "UI/SpaceShooterMenuController.h"

void UHighScoreScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (BackButton != nullptr)
	{
		BackButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnBackButtonClicked);
		BackButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnBackButtonHovered);
	}
}

void UHighScoreScreen::NativeConstruct()
{
	Super::NativeConstruct();
	if (BackButton != nullptr)
	{
		BackButton->SetKeyboardFocus();
	}
}

void UHighScoreScreen::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	if (BackButton != nullptr)
	{
		if (!BackButton->HasKeyboardFocus())
		{
			UE_LOG(LogTemp, Warning, TEXT("UHighScoreScreen::NativeTick - No buttons have keyboard focus. Forcing to BackButton"));
			BackButton->SetKeyboardFocus();
		}
	}
}

void UHighScoreScreen::OnColorShift(FLinearColor LinearColor)
{
	Super::OnColorShift(LinearColor);

	if (HighScoresTextBlock != nullptr)
	{
		HighScoresTextBlock->SetColorAndOpacity(LinearColor);
	}

	SetColorShiftForButton(BackButton, LinearColor);
}

void UHighScoreScreen::OnBackButtonClicked()
{
	USpaceShooterMenuController::OnHighScoreBackClicked.Broadcast();
}

void UHighScoreScreen::OnBackButtonHovered()
{
	if (BackButton != nullptr)
	{
		BackButton->SetKeyboardFocus();
	}
}
