// Copyright 2024 Richard Skala

#include "SpaceShooterSaveGame.h"

FString FHighScoreData::ToString() const
{
	return FString::Printf(TEXT("High Score: %d, DateEarned: %s"), HighScore, *DateEarned);
}

USpaceShooterSaveGame::USpaceShooterSaveGame()
{

}


