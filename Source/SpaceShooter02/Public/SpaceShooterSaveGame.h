// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SpaceShooterSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FHighScoreData
{
	GENERATED_USTRUCT_BODY()

public:
	FString ToString() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 HighScore;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString DateEarned;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 ShipSpriteIndex = -1;
};

UCLASS()
class SPACESHOOTER02_API USpaceShooterSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	USpaceShooterSaveGame();
	const TArray<FHighScoreData>& GetHighScoreDataList() const { return HighScoreDataList; }
	void SetHighScoreDataList(TArray<FHighScoreData> InHighScoreDataList) { HighScoreDataList = InHighScoreDataList; }
	int32 GetHighestSavedScore() const { return HighScoreDataList.Num() > 0 ? HighScoreDataList[0].HighScore : 0; }

	void ResetStats();

	void IncrementNumGamesPlayed() { NumGamesPlayed += 1; }
	void AddNumEnemiesDefeated(int32 InNumEnemiesDefeated) { NumEnemiesDefeated += InNumEnemiesDefeated; }
	void AddNumScoreMultipliersCollected(int32 InNumScoreMultipliersCollected) { NumScoreMultipliersCollected += InNumScoreMultipliersCollected; }
	void AddNumEnemiesDefeatedWithBoost(int32 InNumEnemiesDefeatedWithBoost) { NumEnemiesDefeatedWithBoost += InNumEnemiesDefeatedWithBoost; }
	void IncrementShipSelectedCount(int32 SelectedShipIndex);
	void AddTimeSpentLookingAtStats(float InTimeSpentLookingAtStats) { TimeSpentLookingAtStats += InTimeSpentLookingAtStats; }

private:
	// List of high score data
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<FHighScoreData> HighScoreDataList;

	// Total number of games played
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	int32 NumGamesPlayed = 0;

	// Total number of enemies defeated through every game played
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	int32 NumEnemiesDefeated = 0;

	// Total number of pickups / score multipliers collected
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	int32 NumScoreMultipliersCollected = 0;

	// Total enemies defeated with boost / dash
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	int32 NumEnemiesDefeatedWithBoost = 0;

	// Number of games played with each ship
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TMap<int32, int32> ShipIndexToNumTimesSelected;

	// Time spent looking at stats
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float TimeSpentLookingAtStats = 0.0f;

	friend class USpaceShooterGameInstance;
};
