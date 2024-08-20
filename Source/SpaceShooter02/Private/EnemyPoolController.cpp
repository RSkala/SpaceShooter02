// Copyright 2024 Richard Skala

#include "EnemyPoolController.h"

#include "EnemyBase.h"
#include "EnemyPoolContainer.h"

void UEnemyPoolController::BeginDestroy()
{
	Super::BeginDestroy();
}

void UEnemyPoolController::InitEnemyPools()
{
	if (EnemyClasses.Num() <= 0)
	{
		return;
	}

	for (TSubclassOf<AEnemyBase> EnemyClass : EnemyClasses)
	{
		if (ensureAlways(EnemyClass != nullptr))
		{
			// Create a new pool
			UEnemyPoolContainer* EnemyPoolContainer = NewObject<UEnemyPoolContainer>(this, UEnemyPoolContainer::StaticClass());
			if (EnemyPoolContainer != nullptr)
			{
				EnemyPoolContainers.Add(EnemyPoolContainer);
				EnemyPoolContainer->InitEnemyPool(EnemyClass, MAX_ENEMIES_PER_POOL);
			}
		}
	}
}

void UEnemyPoolController::ResetEnemyPools()
{
	for (UEnemyPoolContainer* EnemyPoolContainer : EnemyPoolContainers)
	{
		if (EnemyPoolContainer != nullptr)
		{
			EnemyPoolContainer->ResetEnemyPool();
		}
	}
}

AEnemyBase* UEnemyPoolController::GetRandomEnemy()
{
	AEnemyBase* Enemy = nullptr;
	int32 RandomIndex = FMath::RandRange(0, EnemyPoolContainers.Num() - 1);
	UEnemyPoolContainer* PoolContainer = EnemyPoolContainers[RandomIndex];
	if (PoolContainer != nullptr)
	{
		Enemy = PoolContainer->GetInactiveEnemy();
	}
	return Enemy;
}
