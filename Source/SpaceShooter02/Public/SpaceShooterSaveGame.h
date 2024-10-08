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
	FHighScoreData()
		: HighScore(0)
		, DateEarned(TEXT(""))
		, ShipSpriteIndex(-1)
	{
	}

	FHighScoreData(int32 InHighScore, FString InDateEarned, int32 InShipSpriteIndex)
		: HighScore(InHighScore)
		, DateEarned(InDateEarned)
		, ShipSpriteIndex(InShipSpriteIndex)
	{
	}

	FString ToString() const;

public:

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

	void AddNumProjectilesFired(int32 InNumProjectilesFired) { NumProjectilesFired += InNumProjectilesFired; }
	void SetHighestScoreMultiplier(int32 InHighestScoreMultiplier) { HighestScoreMultiplier = InHighestScoreMultiplier; }

	void SetLongestGameplaySession(float InLongestGameplaySession) { LongestGameplaySession = InLongestGameplaySession; }

	void IncrementShipSelectedCount(int32 SelectedShipIndex);
	void AddTimeSpentLookingAtStats(float InTimeSpentLookingAtStats) { TimeSpentLookingAtStats += InTimeSpentLookingAtStats; }

	void SetMusicSelection(uint8 InMusicSelection) { MusicSelection = InMusicSelection; }
	void SetSoundEffectsEnabled(bool bInSoundEffectsEnabled) { bSoundEffectsEnabled = bInSoundEffectsEnabled; }
	void SetVOEnabled(bool bInVOEnabled) { bVOEnabled = bInVOEnabled; }

	bool IsShipIndexValid(int32 ShipIndex) const;

private:

	// -- High Score Data ---

	// List of high score data
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<FHighScoreData> HighScoreDataList;

	// --- Game / Player Statistics ---

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

	// Total number of projectiles fired
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	int32 NumProjectilesFired = 0;

	// Highest Score Multiplier Achieved
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	int32 HighestScoreMultiplier = 0;

	//  Longest Gameplay Session
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float LongestGameplaySession = 0.0f;

	// Number of games played with each ship
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TMap<int32, int32> ShipIndexToNumTimesSelected;

	// Time spent looking at stats
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float TimeSpentLookingAtStats = 0.0f;

	// --- Various Options ---

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	uint8 MusicSelection = 4; // EMusicSelection::Random

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool bSoundEffectsEnabled = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool bVOEnabled = true;

	friend class USpaceShooterGameInstance;
};
