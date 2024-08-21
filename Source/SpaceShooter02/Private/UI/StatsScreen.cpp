// Copyright 2024 Richard Skala

#include "UI/StatsScreen.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

#include "SpaceShooterGameInstance.h"
#include "UI/SpaceShooterMenuController.h"

void UStatsScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (BackButton != nullptr)
	{
		BackButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnBackButtonClicked);
	}

	if (USpaceShooterGameInstance* GameInstance = Cast<USpaceShooterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		int32 NumGamesPlayed, NumEnemiesDefeated, NumScoreMultipliersCollected, NumEnemiesDefeatedWithBoost;
		//float SavedTimeSpentLookingAtStats;
		TMap<int32, int32> ShipIndexToNumTimesSelected;
		GameInstance->GetSaveGameStatsData(
			NumGamesPlayed,
			NumEnemiesDefeated,
			NumScoreMultipliersCollected,
			NumEnemiesDefeatedWithBoost,
			SavedTimeSpentLookingAtStats,
			ShipIndexToNumTimesSelected);

		if (NumGamesPlayedTextBlock != nullptr)
		{
			NumGamesPlayedTextBlock->SetText(FText::FromString(FString::Printf(TEXT("Games Played: %d"), NumGamesPlayed)));
		}

		if (NumEnemiesDefeatedTextBlock != nullptr)
		{
			NumEnemiesDefeatedTextBlock->SetText(FText::FromString(FString::Printf(TEXT("Enemies Defeated: %d"), NumEnemiesDefeated)));
		}

		if (NumScoreMultipliersCollectedTextBlock != nullptr)
		{
			NumScoreMultipliersCollectedTextBlock->SetText(FText::FromString(FString::Printf(TEXT("Score Multipliers Collected: %d"), NumScoreMultipliersCollected)));
		}

		if (NumEnemiesDefeatedWithBoostTextBlock != nullptr)
		{
			NumEnemiesDefeatedWithBoostTextBlock->SetText(FText::FromString(FString::Printf(TEXT("Enemies Defeated With Boost: %d"), NumEnemiesDefeatedWithBoost)));
		}

		// NumTimesSelectedShip1TextBlock
		// TODO: Selected ship count

		if (NumTimesSelectedShip1TextBlock != nullptr)
		{
			if (ShipIndexToNumTimesSelected.Contains(0))
			{
				int32 NumTimesPlayed = ShipIndexToNumTimesSelected[0];
				NumTimesSelectedShip1TextBlock->SetText(FText::FromString(FString::Printf(TEXT("Games Played with Ship01: %d"), NumTimesPlayed)));
			}
		}

		if (NumTimesSelectedShip2TextBlock != nullptr)
		{
			if (ShipIndexToNumTimesSelected.Contains(1))
			{
				int32 NumTimesPlayed = ShipIndexToNumTimesSelected[1];
				NumTimesSelectedShip2TextBlock->SetText(FText::FromString(FString::Printf(TEXT("Games Played with Ship02: %d"), NumTimesPlayed)));
			}
		}

		if (NumTimesSelectedShip3TextBlock != nullptr)
		{
			if (ShipIndexToNumTimesSelected.Contains(2))
			{
				int32 NumTimesPlayed = ShipIndexToNumTimesSelected[2];
				NumTimesSelectedShip3TextBlock->SetText(FText::FromString(FString::Printf(TEXT("Games Played with Ship03: %d"), NumTimesPlayed)));
			}
		}

		if (NumTimesSelectedShip4TextBlock != nullptr)
		{
			if (ShipIndexToNumTimesSelected.Contains(3))
			{
				int32 NumTimesPlayed = ShipIndexToNumTimesSelected[3];
				NumTimesSelectedShip4TextBlock->SetText(FText::FromString(FString::Printf(TEXT("Games Played with Ship04: %d"), NumTimesPlayed)));
			}
		}

		if (NumTimesSelectedShip5TextBlock != nullptr)
		{
			if (ShipIndexToNumTimesSelected.Contains(4))
			{
				int32 NumTimesPlayed = ShipIndexToNumTimesSelected[4];
				NumTimesSelectedShip5TextBlock->SetText(FText::FromString(FString::Printf(TEXT("Games Played with Ship05: %d"), NumTimesPlayed)));
			}
		}

		if (NumEnemiesDefeatedWithBoostTextBlock != nullptr)
		{
			NumEnemiesDefeatedWithBoostTextBlock->SetText(FText::FromString(FString::Printf(TEXT("Num Enemies Defeated With Boost: %d"), NumEnemiesDefeatedWithBoost)));
		}
	}

	TimeSpentLookingAtStats = 0;
}

void UStatsScreen::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	TimeSpentLookingAtStats += InDeltaTime;

	if (TimeSpentLookingAtStatsTextBlock != nullptr)
	{
		float TotalTimeSpentLookingAtStats = TimeSpentLookingAtStats + SavedTimeSpentLookingAtStats;
		//UE_LOG(LogTemp, Warning, TEXT("TotalTimeSpentLookingAtStats: %f"), TotalTimeSpentLookingAtStats);
		
		TimeSpentLookingAtStatsTextBlock->SetText(FText::FromString(FString::Printf(TEXT("Time Spent Looking at Stats: %f"), TotalTimeSpentLookingAtStats)));
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
