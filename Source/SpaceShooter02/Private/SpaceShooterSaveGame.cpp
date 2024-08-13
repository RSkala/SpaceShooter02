// Copyright 2024 Richard Skala

#include "SpaceShooterSaveGame.h"

FString FHighScoreData::ToString() const
{
	return FString::Printf(TEXT("High Score: %d, DateEarned: %s, ShipSpriteIndex: %d"), HighScore, *DateEarned, ShipSpriteIndex);
}

USpaceShooterSaveGame::USpaceShooterSaveGame()
{

}


