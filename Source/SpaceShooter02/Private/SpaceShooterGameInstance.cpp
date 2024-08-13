// Copyright 2024 Richard Skala

#include "SpaceShooterGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "SpaceShooterGameState.h"
#include "SpaceShooterSaveGame.h"

FString USpaceShooterGameInstance::GameVersion = "0.0.1";
TArray<FHighScoreData> USpaceShooterGameInstance::DummyHighScoreData;

DEFINE_LOG_CATEGORY_STATIC(LogSpaceShooterGameInstance, Log, All)

void USpaceShooterGameInstance::RecordHighScore(int32 Score)
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
			.ShipSpriteIndex = -1 // Not yet used
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

		// Save the game to disk (This will be located in Saved/SaveGames)
		bool bSaveGameSuccess = UGameplayStatics::SaveGameToSlot(SpaceShooterSaveGame, DefaultSaveSlotName, DefaultSaveSlotIndex);
		UE_CLOG(!bSaveGameSuccess, LogSpaceShooterGameInstance, Warning, TEXT("Failed to create Save Game"));
	}
}

void USpaceShooterGameInstance::OnStart()
{
	Super::OnStart();
}

void USpaceShooterGameInstance::OnGameEnded(int32 FinalScore)
{
	RecordHighScore(FinalScore);
}

void USpaceShooterGameInstance::InitializeHighScoreData()
{
	if (ensure(SpaceShooterSaveGame != nullptr))
	{
		FHighScoreData EmptyHighScoreData
		{
			.HighScore = 0,
			.DateEarned = GetTodaysDateFormatted(),
			.ShipSpriteIndex = -1 // Not yet used
		};

		TArray<FHighScoreData> HighScoreDataList;
		for (int i = 0; i < MaxNumSaveGameHighScores; ++i)
		{
			HighScoreDataList.Add(EmptyHighScoreData);
		}

		SpaceShooterSaveGame->SetHighScoreDataList(HighScoreDataList);
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
