// Copyright 2024 Richard Skala

#include "UI/StatsScreen.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"

#include "SpaceShooterGameInstance.h"
#include "UI/SpaceShooterMenuController.h"
#include "UI/StatDisplayWidget.h"

void UStatsScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (BackButton != nullptr)
	{
		BackButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnBackButtonClicked);
	}

	if (USpaceShooterGameInstance* GameInstance = Cast<USpaceShooterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		// Get saved stats
		int32 NumGamesPlayed, NumEnemiesDefeated, NumScoreMultipliersCollected, NumEnemiesDefeatedWithBoost;
		TMap<int32, int32> ShipIndexToNumTimesSelected;
		GameInstance->GetSaveGameStatsData(
			NumGamesPlayed,
			NumEnemiesDefeated,
			NumScoreMultipliersCollected,
			NumEnemiesDefeatedWithBoost,
			SavedTimeSpentLookingAtStats,
			ShipIndexToNumTimesSelected);

		if (StatListVerticalBox != nullptr)
		{
			// Clear out the test/alignment objects set up during design time
			StatListVerticalBox->ClearChildren();

			// Create the Stat Display Widgets for each stat
			NumGamesPlayedStatDisplay = CreateWidget<UStatDisplayWidget>(this, StatDisplayWidgetClass);
			NumEnemiesDefeatedStatDisplay = CreateWidget<UStatDisplayWidget>(this, StatDisplayWidgetClass);
			NumScoreMultipliersCollectedStatDisplay = CreateWidget<UStatDisplayWidget>(this, StatDisplayWidgetClass);
			NumEnemiesDefeatedWithBoostStatDisplay = CreateWidget<UStatDisplayWidget>(this, StatDisplayWidgetClass);
			NumTimesSelectedShip1StatDisplay = CreateWidget<UStatDisplayWidget>(this, StatDisplayWidgetClass);
			NumTimesSelectedShip2StatDisplay = CreateWidget<UStatDisplayWidget>(this, StatDisplayWidgetClass);
			NumTimesSelectedShip3StatDisplay = CreateWidget<UStatDisplayWidget>(this, StatDisplayWidgetClass);
			NumTimesSelectedShip4StatDisplay = CreateWidget<UStatDisplayWidget>(this, StatDisplayWidgetClass);
			NumTimesSelectedShip5StatDisplay = CreateWidget<UStatDisplayWidget>(this, StatDisplayWidgetClass);
			TimeSpentLookingAtStatsStatDisplay = CreateWidget<UStatDisplayWidget>(this, StatDisplayWidgetClass);

			// Add each StatDisplay to the vertical box
			StatListVerticalBox->AddChildToVerticalBox(NumGamesPlayedStatDisplay);
			StatListVerticalBox->AddChildToVerticalBox(NumEnemiesDefeatedStatDisplay);
			StatListVerticalBox->AddChildToVerticalBox(NumScoreMultipliersCollectedStatDisplay);
			StatListVerticalBox->AddChildToVerticalBox(NumEnemiesDefeatedWithBoostStatDisplay);
			StatListVerticalBox->AddChildToVerticalBox(NumTimesSelectedShip1StatDisplay);
			StatListVerticalBox->AddChildToVerticalBox(NumTimesSelectedShip2StatDisplay);
			StatListVerticalBox->AddChildToVerticalBox(NumTimesSelectedShip3StatDisplay);
			StatListVerticalBox->AddChildToVerticalBox(NumTimesSelectedShip4StatDisplay);
			StatListVerticalBox->AddChildToVerticalBox(NumTimesSelectedShip5StatDisplay);
			StatListVerticalBox->AddChildToVerticalBox(TimeSpentLookingAtStatsStatDisplay);

			// Add each StatDisplay to the list of stat display widgets
			StatDisplayWidgets.Add(NumGamesPlayedStatDisplay);
			StatDisplayWidgets.Add(NumEnemiesDefeatedStatDisplay);
			StatDisplayWidgets.Add(NumScoreMultipliersCollectedStatDisplay);
			StatDisplayWidgets.Add(NumEnemiesDefeatedWithBoostStatDisplay);
			StatDisplayWidgets.Add(NumTimesSelectedShip1StatDisplay);
			StatDisplayWidgets.Add(NumTimesSelectedShip2StatDisplay);
			StatDisplayWidgets.Add(NumTimesSelectedShip3StatDisplay);
			StatDisplayWidgets.Add(NumTimesSelectedShip4StatDisplay);
			StatDisplayWidgets.Add(NumTimesSelectedShip5StatDisplay);
			StatDisplayWidgets.Add(TimeSpentLookingAtStatsStatDisplay);

			// --- General Stats ---

			if (NumGamesPlayedStatDisplay != nullptr)
			{
				NumGamesPlayedStatDisplay->SetStatNameText(FText::FromString(TEXT("Total Games Played:")));

				FText NumGamesPlayedTextGrouped = UKismetTextLibrary::Conv_IntToText(NumGamesPlayed, false, true);
				NumGamesPlayedStatDisplay->UpdateStatDataText(NumGamesPlayedTextGrouped);
			}

			if (NumEnemiesDefeatedStatDisplay != nullptr)
			{
				NumEnemiesDefeatedStatDisplay->SetStatNameText(FText::FromString(TEXT("Total Enemies Defeated:")));

				FText NumEnemiesDefeatedTextGrouped = UKismetTextLibrary::Conv_IntToText(NumEnemiesDefeated, false, true);
				NumEnemiesDefeatedStatDisplay->UpdateStatDataText(NumEnemiesDefeatedTextGrouped);
			}

			if (NumScoreMultipliersCollectedStatDisplay != nullptr)
			{
				NumScoreMultipliersCollectedStatDisplay->SetStatNameText(FText::FromString(TEXT("Score Multipliers Collected:")));

				FText NumScoreMultipliersCollectedTextGrouped = UKismetTextLibrary::Conv_IntToText(NumScoreMultipliersCollected, false, true);
				NumScoreMultipliersCollectedStatDisplay->UpdateStatDataText(NumScoreMultipliersCollectedTextGrouped);
			}

			if (NumEnemiesDefeatedWithBoostStatDisplay != nullptr)
			{
				NumEnemiesDefeatedWithBoostStatDisplay->SetStatNameText(FText::FromString(TEXT("Enemies Defeated With Boost:")));

				FText NumEnemiesDefeatedWithBoostTextGrouped = UKismetTextLibrary::Conv_IntToText(NumEnemiesDefeatedWithBoost, false, true);
				NumEnemiesDefeatedWithBoostStatDisplay->UpdateStatDataText(NumEnemiesDefeatedWithBoostTextGrouped);
			}
			
			// --- Ship Selection Stats ---

			if (NumTimesSelectedShip1StatDisplay != nullptr)
			{
				if (ShipIndexToNumTimesSelected.Contains(0))
				{
					int32 NumTimesPlayed = ShipIndexToNumTimesSelected[0];
					NumTimesSelectedShip1StatDisplay->SetStatNameText(FText::FromString(TEXT("Games Played with Ship01 ..")));

					FText NumTimesPlayedTextGrouped = UKismetTextLibrary::Conv_IntToText(NumTimesPlayed, false, true);
					NumTimesSelectedShip1StatDisplay->UpdateStatDataText(NumTimesPlayedTextGrouped);

					NumTimesSelectedShip1StatDisplay->SetShipImageSpriteByIndex(0);
				}
			}

			if (NumTimesSelectedShip2StatDisplay != nullptr)
			{
				if (ShipIndexToNumTimesSelected.Contains(1))
				{
					int32 NumTimesPlayed = ShipIndexToNumTimesSelected[1];
					NumTimesSelectedShip2StatDisplay->SetStatNameText(FText::FromString(TEXT("Games Played with Ship02 ..")));

					FText NumTimesPlayedTextGrouped = UKismetTextLibrary::Conv_IntToText(NumTimesPlayed, false, true);
					NumTimesSelectedShip2StatDisplay->UpdateStatDataText(NumTimesPlayedTextGrouped);

					NumTimesSelectedShip2StatDisplay->SetShipImageSpriteByIndex(1);
				}
			}

			if (NumTimesSelectedShip3StatDisplay != nullptr)
			{
				if (ShipIndexToNumTimesSelected.Contains(2))
				{
					int32 NumTimesPlayed = ShipIndexToNumTimesSelected[2];
					NumTimesSelectedShip3StatDisplay->SetStatNameText(FText::FromString(TEXT("Games Played with Ship03 ..")));

					FText NumTimesPlayedTextGrouped = UKismetTextLibrary::Conv_IntToText(NumTimesPlayed, false, true);
					NumTimesSelectedShip3StatDisplay->UpdateStatDataText(NumTimesPlayedTextGrouped);

					NumTimesSelectedShip3StatDisplay->SetShipImageSpriteByIndex(2);
				}
			}

			if (NumTimesSelectedShip4StatDisplay != nullptr)
			{
				if (ShipIndexToNumTimesSelected.Contains(3))
				{
					int32 NumTimesPlayed = ShipIndexToNumTimesSelected[3];
					NumTimesSelectedShip4StatDisplay->SetStatNameText(FText::FromString(TEXT("Games Played with Ship04 ..")));

					FText NumTimesPlayedTextGrouped = UKismetTextLibrary::Conv_IntToText(NumTimesPlayed, false, true);
					NumTimesSelectedShip4StatDisplay->UpdateStatDataText(NumTimesPlayedTextGrouped);

					NumTimesSelectedShip4StatDisplay->SetShipImageSpriteByIndex(3);
				}
			}

			if (NumTimesSelectedShip5StatDisplay != nullptr)
			{
				if (ShipIndexToNumTimesSelected.Contains(4))
				{
					int32 NumTimesPlayed = ShipIndexToNumTimesSelected[4];
					NumTimesSelectedShip5StatDisplay->SetStatNameText(FText::FromString(TEXT("Games Played with Ship05 ..")));

					FText NumTimesPlayedTextGrouped = UKismetTextLibrary::Conv_IntToText(NumTimesPlayed, false, true);
					NumTimesSelectedShip5StatDisplay->UpdateStatDataText(NumTimesPlayedTextGrouped);

					NumTimesSelectedShip5StatDisplay->SetShipImageSpriteByIndex(4);
				}
			}

			// --- Other Stats ---

			if (TimeSpentLookingAtStatsStatDisplay != nullptr)
			{
				TimeSpentLookingAtStatsStatDisplay->SetStatNameText(FText::FromString(TEXT("Time Spent Looking at Stats:")));
				TimeSpentLookingAtStatsStatDisplay->UpdateStatDataText(FText::FromString(FString::Printf(TEXT("%f"), SavedTimeSpentLookingAtStats)));
			}
		}
	}

	TimeSpentLookingAtStats = 0;
}

void UStatsScreen::NativeDestruct()
{
	Super::NativeDestruct();
	
	if (StatListVerticalBox != nullptr)
	{
		StatListVerticalBox->ClearChildren();
	}
}

void UStatsScreen::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	TimeSpentLookingAtStats += InDeltaTime;

	if (TimeSpentLookingAtStatsStatDisplay != nullptr)
	{
		float TotalTimeSpentLookingAtStats = TimeSpentLookingAtStats + SavedTimeSpentLookingAtStats;

		FTimespan TimeSpan = FTimespan::FromSeconds(TotalTimeSpentLookingAtStats);;
		int32 TotalHours = TimeSpan.GetTotalHours();
		int32 Minutes = TimeSpan.GetMinutes();
		int32 Seconds = TimeSpan.GetSeconds();
		int32 Milliseconds = TimeSpan.GetFractionMilli() / 100;

		FString MinutesString = Minutes < 10 ? FString::Printf(TEXT("0%d"), Minutes) : FString::Printf(TEXT("%d"), Minutes);
		FString SecondsString = Seconds < 10 ? FString::Printf(TEXT("0%d"), Seconds) : FString::Printf(TEXT("%d"), Seconds);
		
		TimeSpentLookingAtStatsStatDisplay->UpdateStatDataText(
			FText::FromString(
				FString::Printf(TEXT("%dh%sm%s.%ds"), TotalHours, *MinutesString, *SecondsString, Milliseconds)));
	}
}

void UStatsScreen::OnColorShift(FLinearColor LinearColor)
{
	Super::OnColorShift(LinearColor);
	SetColorShiftForButton(BackButton, LinearColor);

	if (StatsTitleTextBlock != nullptr)
	{
		StatsTitleTextBlock->SetColorAndOpacity(LinearColor);
	}

	for (UStatDisplayWidget* StatDisplayWidget : StatDisplayWidgets)
	{
		if (StatDisplayWidget != nullptr)
		{
			StatDisplayWidget->SetColorShift(LinearColor);
		}
	}
}

void UStatsScreen::OnBackButtonClicked()
{
	USpaceShooterMenuController::OnStatsScreenBackClicked.Broadcast(TimeSpentLookingAtStats);
}

void UStatsScreen::OnBackButtonHovered()
{
	if (BackButton != nullptr)
	{
		BackButton->SetKeyboardFocus();
	}
}
