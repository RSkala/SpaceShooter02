// Copyright 2024 Richard Skala

#include "SpaceShooterGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "AudioEnums.h"
#include "AudioController.h"
#include "SpaceShooterGameState.h"
#include "SpaceShooterSaveGame.h"

FString USpaceShooterGameInstance::GameVersion = "0.0.1";
TArray<FHighScoreData> USpaceShooterGameInstance::DummyHighScoreData;

DEFINE_LOG_CATEGORY_STATIC(LogSpaceShooterGameInstance, Log, All)

void USpaceShooterGameInstance::RecordHighScore(int32 Score, int32 SelectedShipSpriteIndex)
{
	if (SpaceShooterSaveGame == nullptr)
	{
		UE_LOG(LogSpaceShooterGameInstance, Warning, TEXT("Invalid save game object. Data will not be saved."));
		return;
	}

	// Don't bother saving scores of zero
	if (Score <= 0)
	{
		return;
	}

	// Ensure the high score data list has been initialized. It should have already been done, but ensure that it is.
	if (SpaceShooterSaveGame->GetHighScoreDataList().Num() == 0)
	{
		InitializeHighScoreData();
	}

	// Get the high score list and ensure it is sorted in descending order (highest score first)
	TArray<FHighScoreData> HighScoreDataList = SpaceShooterSaveGame->GetHighScoreDataList();
	Algo::Sort(HighScoreDataList, [](const FHighScoreData& A, const FHighScoreData& B)
	{
		return A.HighScore > B.HighScore;
	});

	// Check if this incoming score is higher than any saved scores
	int32 LastHighestScoreIndex = -1;
	for (int32 HighScoreIndex = 0; HighScoreIndex < HighScoreDataList.Num() - 1; ++HighScoreIndex)
	{
		if (Score >= HighScoreDataList[HighScoreIndex].HighScore)
		{
			// Found a lower score at the current index, save the high score list index, as that is where the new score will be inserted
			LastHighestScoreIndex = HighScoreIndex;
			break;
		}
	}

	// Check if a valid index was found (meaning this incoming score is higher than any recorded high score)
	if (LastHighestScoreIndex != -1)
	{
		// The incoming score is higher than any other score in the high score list
		// Create a new High Score Data object to save into the Save Game array
		FHighScoreData NewHighScoreData
		{
			.HighScore = Score,
			.DateEarned = GetTodaysDateFormatted(),
			.ShipSpriteIndex = SelectedShipSpriteIndex
		};

		// Insert the new high score into the slot
		HighScoreDataList.Insert(NewHighScoreData, LastHighestScoreIndex);

		// Ensure the High Score Data List is not larger than the max number of records
		if (HighScoreDataList.Num() > MaxNumSaveGameHighScores)
		{
			// Get the number of elements to trim from the back
			int32 NumElementsToRemove = HighScoreDataList.Num() - MaxNumSaveGameHighScores;
			for (int32 i = HighScoreDataList.Num() - 1; NumElementsToRemove > 0; --NumElementsToRemove)
			{
				HighScoreDataList.RemoveAt(i);
			}
		}

		// Set the new high score data list
		SpaceShooterSaveGame->SetHighScoreDataList(HighScoreDataList);

		// Save the score
		bool bSaveGameSuccess = UGameplayStatics::SaveGameToSlot(SpaceShooterSaveGame, DefaultSaveSlotName, DefaultSaveSlotIndex);
		UE_CLOG(!bSaveGameSuccess, LogSpaceShooterGameInstance, Warning, TEXT("Failed to create Save Game"));

		// Debug: Print the High Scores
		UE_LOG(LogSpaceShooterGameInstance, Log, TEXT("---------------------------------"));
		UE_LOG(LogSpaceShooterGameInstance, Log, TEXT("Printing High Score Data"));
		const TArray<FHighScoreData> UpdatedHighScores = SpaceShooterSaveGame->GetHighScoreDataList();
		for (const FHighScoreData& HighScoreData : UpdatedHighScores)
		{
			UE_LOG(LogSpaceShooterGameInstance, Log, TEXT("%s"), *HighScoreData.ToString());
		}
	}
	else
	{
		UE_LOG(LogSpaceShooterGameInstance, Log, TEXT("High score not updated"));
	}
}

const TArray<struct FHighScoreData>& USpaceShooterGameInstance::GetHighScoreDataList() const
{
	if (SpaceShooterSaveGame != nullptr)
	{
		return SpaceShooterSaveGame->GetHighScoreDataList();
	}
	else
	{
		return DummyHighScoreData;
	}
}

UPaperSprite* USpaceShooterGameInstance::GetShipSpriteForIndex(int32 ShipSpriteIndex) const
{
	if (ShipSpriteIndex < 0 || ShipSpriteIndex >= ShipSprites.Num())
	{
		ensure(InvalidShipSprite != nullptr);
		return InvalidShipSprite;
	}
	else
	{
		return ShipSprites[ShipSpriteIndex];
	}
}

int32 USpaceShooterGameInstance::GetPlayerHighestScore() const
{
	return SpaceShooterSaveGame != nullptr ? SpaceShooterSaveGame->GetHighestSavedScore() : 0;
}

void USpaceShooterGameInstance::ClearHighScores()
{
	if (SpaceShooterSaveGame != nullptr)
	{
		UE_LOG(LogSpaceShooterGameInstance, Log, TEXT("Clearing High Scores"));
		InitializeHighScoreData();

		bool bSaveGameSuccess = UGameplayStatics::SaveGameToSlot(SpaceShooterSaveGame, DefaultSaveSlotName, DefaultSaveSlotIndex);
		UE_CLOG(!bSaveGameSuccess, LogSpaceShooterGameInstance, Warning, TEXT("%s - Failed to save game"), ANSI_TO_TCHAR(__FUNCTION__));
	}
}

void USpaceShooterGameInstance::RecordPostGameStats(
	int32 NumEnemiesDefeated,
	int32 NumScoreMultipliersCollected,
	int32 NumEnemiesDefeatedWithBoost,
	int32 NumProjectilesFired,
	int32 CurrentScoreMultiplier,
	int32 SelectedShipSpriteIndex)
{
	if (SpaceShooterSaveGame != nullptr)
	{
		// Save the score
		SpaceShooterSaveGame->IncrementNumGamesPlayed();
		SpaceShooterSaveGame->AddNumEnemiesDefeated(NumEnemiesDefeated);
		SpaceShooterSaveGame->AddNumScoreMultipliersCollected(NumScoreMultipliersCollected);
		SpaceShooterSaveGame->AddNumEnemiesDefeatedWithBoost(NumEnemiesDefeatedWithBoost);
		SpaceShooterSaveGame->IncrementShipSelectedCount(SelectedShipSpriteIndex);

		SpaceShooterSaveGame->AddNumProjectilesFired(NumProjectilesFired);

		if (CurrentScoreMultiplier > SpaceShooterSaveGame->HighestScoreMultiplier)
		{
			SpaceShooterSaveGame->SetHighestScoreMultiplier(CurrentScoreMultiplier);
		}

		bool bSaveGameSuccess = UGameplayStatics::SaveGameToSlot(SpaceShooterSaveGame, DefaultSaveSlotName, DefaultSaveSlotIndex);
		UE_CLOG(!bSaveGameSuccess, LogSpaceShooterGameInstance, Warning, TEXT("%s - Failed to save game"), ANSI_TO_TCHAR(__FUNCTION__));
	}
}

void USpaceShooterGameInstance::ClearStats()
{
	if (SpaceShooterSaveGame != nullptr)
	{
		UE_LOG(LogSpaceShooterGameInstance, Log, TEXT("Clearing Stats"));
		InitializeStatsData();

		bool bSaveGameSuccess = UGameplayStatics::SaveGameToSlot(SpaceShooterSaveGame, DefaultSaveSlotName, DefaultSaveSlotIndex);
		UE_CLOG(!bSaveGameSuccess, LogSpaceShooterGameInstance, Warning, TEXT("%s - Failed to save data"), ANSI_TO_TCHAR(__FUNCTION__));
	}
}

void USpaceShooterGameInstance::GetSaveGameStatsData(
	int32& OutNumGamesPlayed,
	int32& OutNumEnemiesDefeated,
	int32& OutNumScoreMultipliersCollected,
	int32& OutNumEnemiesDefeatedWithBoost,
	int32& OutNumProjectilesFired,
	int32& OutHighestScoreMultiplier,
	float& OutTimeSpentLookingAtStats,
	TMap<int32, int32>& OutShipIndexToNumTimesSelected)
{
	if (SpaceShooterSaveGame != nullptr)
	{
		OutNumGamesPlayed = SpaceShooterSaveGame->NumGamesPlayed;
		OutNumEnemiesDefeated = SpaceShooterSaveGame->NumEnemiesDefeated;
		OutNumScoreMultipliersCollected = SpaceShooterSaveGame->NumScoreMultipliersCollected;
		OutNumEnemiesDefeatedWithBoost = SpaceShooterSaveGame->NumEnemiesDefeatedWithBoost;
		OutNumProjectilesFired = SpaceShooterSaveGame->NumProjectilesFired;
		OutHighestScoreMultiplier = SpaceShooterSaveGame->HighestScoreMultiplier;
		OutTimeSpentLookingAtStats = SpaceShooterSaveGame->TimeSpentLookingAtStats;
		OutShipIndexToNumTimesSelected = SpaceShooterSaveGame->ShipIndexToNumTimesSelected;
	}
}

void USpaceShooterGameInstance::SaveTimeSpentLookingAtStats(float InTimeSpentLookingAtStats)
{
	if (SpaceShooterSaveGame != nullptr)
	{
		SpaceShooterSaveGame->AddTimeSpentLookingAtStats(InTimeSpentLookingAtStats);

		bool bSaveGameSuccess = UGameplayStatics::SaveGameToSlot(SpaceShooterSaveGame, DefaultSaveSlotName, DefaultSaveSlotIndex);
		UE_CLOG(!bSaveGameSuccess, LogSpaceShooterGameInstance, Warning, TEXT("%s - Failed to save game"), ANSI_TO_TCHAR(__FUNCTION__));
	}
}

void USpaceShooterGameInstance::PlayGameplayMusic()
{
	if (AudioController != nullptr)
	{
		EMusicSelection MusicSelection = EMusicSelection::Random;
		if (SpaceShooterSaveGame != nullptr)
		{
			MusicSelection = static_cast<EMusicSelection>(SpaceShooterSaveGame->MusicSelection);
		}
		AudioController->PlayGameplayMusic(MusicSelection);
	}
}

void USpaceShooterGameInstance::StopGameplayMusic()
{
	if (AudioController != nullptr)
	{
		AudioController->StopGameplayMusicImmediately();
	}
}

void USpaceShooterGameInstance::FadeOutGameplayMusic()
{
	if (AudioController != nullptr)
	{
		AudioController->FadeOutGameplayMusic();
	}
}

void USpaceShooterGameInstance::OnCycleMusicSelection()
{
	if (SpaceShooterSaveGame != nullptr)
	{
		uint8 MusicSelection = SpaceShooterSaveGame->MusicSelection;
		MusicSelection++;
		if (MusicSelection >= static_cast<uint8>(EMusicSelection::NumMusicTracks))
		{
			MusicSelection = static_cast<uint8>(EMusicSelection::Track1);
		}
		SpaceShooterSaveGame->SetMusicSelection(MusicSelection);
	}
}

void USpaceShooterGameInstance::OnCycleSoundEffectOption()
{
	if (SpaceShooterSaveGame != nullptr)
	{
		SpaceShooterSaveGame->SetSoundEffectsEnabled(!SpaceShooterSaveGame->bSoundEffectsEnabled);
	}
}

void USpaceShooterGameInstance::SaveAudioOptionData()
{
	bool bSaveGameSuccess = UGameplayStatics::SaveGameToSlot(SpaceShooterSaveGame, DefaultSaveSlotName, DefaultSaveSlotIndex);
	UE_CLOG(!bSaveGameSuccess, LogSpaceShooterGameInstance, Warning, TEXT("%s - Failed to save game"), ANSI_TO_TCHAR(__FUNCTION__));
}

EMusicSelection USpaceShooterGameInstance::GetMusicSelection() const
{
	EMusicSelection MusicSelection = EMusicSelection::Random;
	if (SpaceShooterSaveGame != nullptr)
	{
		MusicSelection = static_cast<EMusicSelection>(SpaceShooterSaveGame->MusicSelection);
	}
	return MusicSelection;
}

bool USpaceShooterGameInstance::GetSoundEffectsEnabled() const
{
	bool bSoundEffectsEnabled = true;
	if (SpaceShooterSaveGame != nullptr)
	{
		bSoundEffectsEnabled = SpaceShooterSaveGame->bSoundEffectsEnabled;
	}
	return bSoundEffectsEnabled;
}

void USpaceShooterGameInstance::PlaySound(ESoundEffect SoundEffect)
{
	if (AudioController != nullptr)
	{
		AudioController->PlaySound(SoundEffect);
	}
}

void USpaceShooterGameInstance::PlayMenuVO(EMenuSoundVO MenuSoundVO)
{
	if (AudioController != nullptr)
	{
		AudioController->PlayMenuVO(MenuSoundVO);
	}
}

void USpaceShooterGameInstance::Init()
{
	Super::Init();

	// Get notified when the game ends
	ASpaceShooterGameState::OnGameEnded.AddUniqueDynamic(this, &ThisClass::OnGameEnded);

	// Create or load save game
	if (UGameplayStatics::DoesSaveGameExist(DefaultSaveSlotName, DefaultSaveSlotIndex))
	{
		// Save game exists. Load the save game object
		SpaceShooterSaveGame = Cast<USpaceShooterSaveGame>(UGameplayStatics::LoadGameFromSlot(DefaultSaveSlotName, 0));
		ensure(SpaceShooterSaveGame != nullptr);
	}
	else
	{
		// Save game does not exist. Create a save game object.
		SpaceShooterSaveGame = Cast<USpaceShooterSaveGame>(UGameplayStatics::CreateSaveGameObject(USpaceShooterSaveGame::StaticClass()));
		ensure(SpaceShooterSaveGame != nullptr);

		// Initialize the high score data list
		InitializeHighScoreData();

		// Initialize stats data
		InitializeStatsData();

		// Save the game to disk (This will be located in Saved/SaveGames)
		bool bSaveGameSuccess = UGameplayStatics::SaveGameToSlot(SpaceShooterSaveGame, DefaultSaveSlotName, DefaultSaveSlotIndex);
		UE_CLOG(!bSaveGameSuccess, LogSpaceShooterGameInstance, Warning, TEXT("Failed to create Save Game"));
	}

	// Create the Audio Controller
	if (ensure(AudioControllerClass != nullptr))
	{
		AudioController = NewObject<UAudioController>(this, AudioControllerClass);
		ensure(AudioController != nullptr);
	}
}

void USpaceShooterGameInstance::OnStart()
{
	Super::OnStart();
}

void USpaceShooterGameInstance::OnGameEnded(
	int32 FinalScore,
	int32 SelectedShipSpriteIndex,
	int32 NumEnemiesDefeated,
	int32 NumScoreMultipliersCollected,
	int32 NumEnemiesDefeatedWithBoost,
	int32 NumProjectilesFired,
	int32 CurrentScoreMultiplier)
{
	RecordHighScore(FinalScore, SelectedShipSpriteIndex);
	RecordPostGameStats(
		NumEnemiesDefeated,
		NumScoreMultipliersCollected,
		NumEnemiesDefeatedWithBoost,
		NumProjectilesFired,
		CurrentScoreMultiplier,
		SelectedShipSpriteIndex);
}

void USpaceShooterGameInstance::InitializeHighScoreData()
{
	if (ensure(SpaceShooterSaveGame != nullptr))
	{
		FHighScoreData EmptyHighScoreData
		{
			.HighScore = 0,
			.DateEarned = GetTodaysDateFormatted(),
			.ShipSpriteIndex = INVALID_SHIP_INDEX
		};

		TArray<FHighScoreData> HighScoreDataList;
		for (int i = 0; i < MaxNumSaveGameHighScores; ++i)
		{
			HighScoreDataList.Add(EmptyHighScoreData);
		}

		SpaceShooterSaveGame->SetHighScoreDataList(HighScoreDataList);
	}
}

void USpaceShooterGameInstance::InitializeStatsData()
{
	if (ensure(SpaceShooterSaveGame != nullptr))
	{
		SpaceShooterSaveGame->ResetStats();
	}
}

FString USpaceShooterGameInstance::GetTodaysDateFormatted() const
{
	// Get today's date (only year, month, day)
	int32 Year, Month, Day;
	FDateTime::Today().GetDate(Year, Month, Day);

	// Create a date string formatted as: YYYY.MM.DD
	FString YearString = FString::Printf(TEXT("%d"), Year);
	FString MonthString = Month < 10 ? FString::Printf(TEXT("0%d"), Month) : FString::Printf(TEXT("%d"), Month);
	FString DayString = Day < 10 ? FString::Printf(TEXT("0%d"), Day) : FString::Printf(TEXT("%d"), Day);

	// Return the formatted date string
	return FString::Printf(TEXT("%s.%s.%s"), *YearString, *MonthString, *DayString);
}
