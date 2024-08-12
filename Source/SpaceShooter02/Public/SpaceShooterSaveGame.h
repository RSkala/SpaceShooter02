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
	TArray<FHighScoreData> GetHighScoreDataList() const { return HighScoreDataList; }
	void SetHighScoreDataList(TArray<FHighScoreData> InHighScoreDataList) { HighScoreDataList = InHighScoreDataList; }

private:
	// List of high score data
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<FHighScoreData> HighScoreDataList;
};
