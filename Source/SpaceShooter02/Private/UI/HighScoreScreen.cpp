// Copyright 2024 Richard Skala

#include "UI/HighScoreScreen.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"

#include "SpaceShooterGameInstance.h"
#include "SpaceShooterSaveGame.h"
#include "UI/SpaceShooterMenuController.h"
#include "UI/ScoreDisplayWidget.h"

void UHighScoreScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (BackButton != nullptr)
	{
		BackButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnBackButtonClicked);
		BackButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnBackButtonHovered);
	}

	if (HighScoreListVerticalBox != nullptr)
	{
		// Clear out the test/alignment objects set up during design time
		HighScoreListVerticalBox->ClearChildren();

		// Get the High Score data from the GameInstance
		if (USpaceShooterGameInstance* SpaceShooterGameInstance = Cast<USpaceShooterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
		{
			if (ensure(ScoreDisplayWidgetClass != nullptr))
			{
				// Iterate through the high score data list and create and init the high score display widgets
				int32 RankCount = 1; // "Rank" starts at 1 and the high score list is already sorted
				const TArray<FHighScoreData>& HighScoreDataList = SpaceShooterGameInstance->GetHighScoreDataList();
				for (const FHighScoreData& HighScoreData : HighScoreDataList)
				{
					UScoreDisplayWidget* ScoreDisplayWidget = CreateWidget<UScoreDisplayWidget>(this, ScoreDisplayWidgetClass);
					if (ensure(ScoreDisplayWidget != nullptr))
					{
						ScoreDisplayWidget->SetScoreInfoFromScoreData(HighScoreData, RankCount);
						HighScoreListVerticalBox->AddChildToVerticalBox(ScoreDisplayWidget);
						ScoreDisplayWidgets.Add(ScoreDisplayWidget);
						++RankCount;
					}
				}
			}
		}
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

void UHighScoreScreen::NativeDestruct()
{
	Super::NativeDestruct();

	// Empty the score display widgets array. This should in theory call their destructors.
	ScoreDisplayWidgets.Empty();
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

	for (UScoreDisplayWidget* ScoreDisplayWidget : ScoreDisplayWidgets)
	{
		ScoreDisplayWidget->SetColorShift(LinearColor);
	}
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
