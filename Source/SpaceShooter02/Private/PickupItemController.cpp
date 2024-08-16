// Copyright 2024 Richard Skala

#include "PickupItemController.h"

#include "PickupItemScoreMultiplier.h"

DEFINE_LOG_CATEGORY_STATIC(LogPickupItemController, Log, All)

void UPickupItemController::InitScoreMultiplierPool()
{
	for (int i = 0; i < MAX_SCORE_MULTIPLIERS; ++i)
	{
		CreateAndAddNewScoreMultiplier();
	}
}

void UPickupItemController::ResetScoreMultiplierPool()
{
	// Iterate through the entire score multiplier pool and deactivate each
	for (APickupItemScoreMultiplier* const ScoreMultiplier : ScoreMultiplierPool)
	{
		if (ScoreMultiplier != nullptr)
		{
			ScoreMultiplier->DeactivatePoolObject();
		}
	}
}

APickupItemScoreMultiplier* UPickupItemController::GetInactiveScoreMultiplier()
{
	// Get first inactive projectile
	APickupItemScoreMultiplier* InactivePickupItem = nullptr;
	for (APickupItemScoreMultiplier* const ScoreMultiplier : ScoreMultiplierPool)
	{
		if (ScoreMultiplier != nullptr && !ScoreMultiplier->IsPoolObjectActive())
		{
			// Found an inactive score multiplier
			InactivePickupItem = ScoreMultiplier;
			break;
		}
	}

	if (InactivePickupItem == nullptr)
	{
		UE_LOG(LogPickupItemController, Warning, TEXT("No available score multipliers in the pool. Increase the pool size."));
		InactivePickupItem = CreateAndAddNewScoreMultiplier();
	}
	return InactivePickupItem;
}

APickupItemScoreMultiplier* UPickupItemController::CreateAndAddNewScoreMultiplier()
{
	APickupItemScoreMultiplier* NewScoreMultiplier = nullptr;
	if (ensure(ScoreMultiplierClass != nullptr))
	{
		if (UWorld* World = GetWorld())
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Name = TEXT("ScoreMultiplier");
			//ProjectileSpawnParameters.Instigator = this;
			SpawnParameters.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			NewScoreMultiplier = World->SpawnActor<APickupItemScoreMultiplier>(ScoreMultiplierClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParameters);
			if (NewScoreMultiplier != nullptr)
			{
				ScoreMultiplierPool.Add(NewScoreMultiplier);
			}
			else
			{
				UE_LOG(LogPickupItemController, Warning, TEXT("Failed to create score multiplier pickup."));
			}
		}
	}
	return NewScoreMultiplier;
}
