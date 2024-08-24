// Copyright 2024 Richard Skala

#include "SpaceShooterSaveGame.h"

DEFINE_LOG_CATEGORY_STATIC(LogSpaceShooterSaveGame, Log, All)

FString FHighScoreData::ToString() const
{
	return FString::Printf(TEXT("High Score: %d, DateEarned: %s, ShipSpriteIndex: %d"), HighScore, *DateEarned, ShipSpriteIndex);
}

USpaceShooterSaveGame::USpaceShooterSaveGame()
{

}

void USpaceShooterSaveGame::ResetStats()
{
	UE_LOG(LogSpaceShooterSaveGame, Log, TEXT("USpaceShooterSaveGame::ResetStats"));

	NumGamesPlayed = 0;
	NumEnemiesDefeated = 0;
	NumScoreMultipliersCollected = 0;
	NumEnemiesDefeatedWithBoost = 0;
	NumProjectilesFired = 0;
	HighestScoreMultiplier = 1;
	LongestGameplaySession = 0.0f;

	ShipIndexToNumTimesSelected.Empty();
	ShipIndexToNumTimesSelected.Add(0, 0); // Ship 1 (White)
	ShipIndexToNumTimesSelected.Add(1, 0); // Ship 2 (Orange)
	ShipIndexToNumTimesSelected.Add(2, 0); // Ship 3 (Green)
	ShipIndexToNumTimesSelected.Add(3, 0); // Ship 4 (Purple)
	ShipIndexToNumTimesSelected.Add(4, 0); // Ship 5 (Red)

	TimeSpentLookingAtStats = 0.0f;
}

void USpaceShooterSaveGame::IncrementShipSelectedCount(int32 SelectedShipIndex)
{
	if (!ShipIndexToNumTimesSelected.Contains(SelectedShipIndex))
	{
		UE_LOG(
			LogSpaceShooterSaveGame,
			Warning,
			TEXT("USpaceShooterSaveGame::IncrementShipSelectedCount - ShipIndexToNumTimesSelected does NOT contain index %d. Adding if valid."), SelectedShipIndex);

		if (IsShipIndexValid(SelectedShipIndex))
		{
			ShipIndexToNumTimesSelected.Add(SelectedShipIndex, 0);
		}
	}

	if (ensure(ShipIndexToNumTimesSelected.Contains(SelectedShipIndex)))
	{
		int32 OldVal = ShipIndexToNumTimesSelected[SelectedShipIndex];
		ShipIndexToNumTimesSelected[SelectedShipIndex] = OldVal + 1;
	}
}

bool USpaceShooterSaveGame::IsShipIndexValid(int32 ShipIndex) const
{
	if (ShipIndex >= 0 && ShipIndex <= 4)
	{
		// Ship Index should be 0, 1, 2, 3, 4
		return true;
	}
	return false;
}


