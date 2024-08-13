// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SpaceShooterGameInstance.generated.h"

UCLASS()
class SPACESHOOTER02_API USpaceShooterGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	void RecordHighScore(int32 Score, int32 SelectedShipSpriteIndex);
	const TArray<struct FHighScoreData>& GetHighScoreDataList() const;
	class UPaperSprite* GetShipSpriteForIndex(int32 ShipSpriteIndex) const;

	static FString GetGameVersionString() { return GameVersion; }

protected:
	virtual void Init() override;
	virtual void OnStart() override;

private:
	UFUNCTION()
	void OnGameEnded(int32 FinalScore, int32 SelectedShipSpriteIndex);

	// Initialize the high score data list with empty data
	void InitializeHighScoreData();

	FString GetTodaysDateFormatted() const;

public:
	static constexpr int32 INVALID_SHIP_INDEX = -1;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FString DefaultSaveSlotName = "SaveSlot1";

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	int32 DefaultSaveSlotIndex = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class USpaceShooterSaveGame> SpaceShooterSaveGame;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<class UPaperSprite>> ShipSprites;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UPaperSprite> InvalidShipSprite;

	static FString GameVersion;
	static constexpr int32 MaxNumSaveGameHighScores = 15;

	// Failsafe in case SpaceShooterSaveGame is invalid
	static TArray<struct FHighScoreData> DummyHighScoreData;
};
