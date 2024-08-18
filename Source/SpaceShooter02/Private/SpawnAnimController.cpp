// Copyright 2024 Richard Skala

#include "SpawnAnimController.h"

#include "SpawnAnimBase.h"

void USpawnAnimController::InitSpawnAnimPool()
{
	for (TSubclassOf<class ASpawnAnimBase> SpawnAnimClass : SpawnAnimClasses)
	{
		if (SpawnAnimClass != nullptr)
		{
			int32 NumIterationsPerClass = static_cast<int32>(MAX_SPAWN_ANIMS / SpawnAnimClasses.Num());
			for (int32 i = 0; i < NumIterationsPerClass; ++i)
			{
				CreateAndAddNewSpawnAnim(SpawnAnimClass);
			}
		}
	}

	// Shuffle the pool
	// https://forums.unrealengine.com/t/how-to-shuffle-a-tarray/286257/6
	int64 DateInSeconds = FDateTime::Now().ToUnixTimestamp();
	FRandomStream SRand = FRandomStream();
	SRand.Initialize(DateInSeconds);

	SpawnAnimPool.Sort([&SRand](const ASpawnAnimBase& A, const ASpawnAnimBase& B)
	{
		return SRand.GetFraction() < 0.5f;
	});
}

void USpawnAnimController::ResetSpawnAnimPool()
{
	for (ASpawnAnimBase* SpawnAnim : SpawnAnimPool)
	{
		if (SpawnAnim != nullptr)
		{
			SpawnAnim->DeactivatePoolObject();
		}
	}
}

ASpawnAnimBase* USpawnAnimController::GetInactiveSpawnAnim()
{
	ASpawnAnimBase* InactiveSpawnAnim = nullptr;
	for (ASpawnAnimBase* SpawnAnim : SpawnAnimPool)
	{
		if (SpawnAnim != nullptr && !SpawnAnim->IsPoolObjectActive())
		{
			InactiveSpawnAnim = SpawnAnim;
			break;
		}
	}
	if (InactiveSpawnAnim == nullptr)
	{
		if (SpawnAnimClasses.Num() > 0)
		{
			int32 RandomIndex = FMath::RandRange(0, SpawnAnimClasses.Num() - 1);
			TSubclassOf<ASpawnAnimBase> SpawnAnimClass = SpawnAnimClasses[RandomIndex];
			InactiveSpawnAnim = CreateAndAddNewSpawnAnim(SpawnAnimClass);
		}
	}
	return InactiveSpawnAnim;
}

ASpawnAnimBase* USpawnAnimController::CreateAndAddNewSpawnAnim(TSubclassOf<class ASpawnAnimBase> SpawnAnimClass)
{
	ASpawnAnimBase* NewSpawnAnim = nullptr;
	if (SpawnAnimClass != nullptr)
	{
		if (UWorld* World = GetWorld())
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Name = TEXT("EnemySpawnAnim");
			SpawnParams.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			NewSpawnAnim = World->SpawnActor<ASpawnAnimBase>(SpawnAnimClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
			if (NewSpawnAnim != nullptr)
			{
				SpawnAnimPool.Add(NewSpawnAnim);
			}
		}
	}
	return NewSpawnAnim;
}
