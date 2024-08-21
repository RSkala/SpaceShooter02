// Copyright 2024 Richard Skala

#include "SpaceShooterSaveGame.h"

FString FHighScoreData::ToString() const
{
	return FString::Printf(TEXT("High Score: %d, DateEarned: %s, ShipSpriteIndex: %d"), HighScore, *DateEarned, ShipSpriteIndex);
}

USpaceShooterSaveGame::USpaceShooterSaveGame()
{

}

void USpaceShooterSaveGame::ResetStats()
{
	NumGamesPlayed = 0;
	NumEnemiesDefeated = 0;
	NumScoreMultipliersCollected = 0;
	NumEnemiesDefeatedWithBoost = 0;
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
	if (ensure(ShipIndexToNumTimesSelected.Contains(SelectedShipIndex)))
	{
		int32 OldVal = ShipIndexToNumTimesSelected[SelectedShipIndex];
		ShipIndexToNumTimesSelected[SelectedShipIndex] = OldVal + 1;
	}
}


