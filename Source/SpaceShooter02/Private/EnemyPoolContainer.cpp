// Copyright 2024 Richard Skala

#include "EnemyPoolContainer.h"

#include "EnemyBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogEnemyPoolContainer, Log, All)

void UEnemyPoolContainer::BeginDestroy()
{
	Super::BeginDestroy();
}

void UEnemyPoolContainer::InitEnemyPool(TSubclassOf<class AEnemyBase> InEnemyClass, int32 MaxNumEnemies)
{
	EnemyClass = InEnemyClass;
	for (int32 i = 0; i < MaxNumEnemies; ++i)
	{
		CreateAndAddEnemyToPool();
	}
}

void UEnemyPoolContainer::ResetEnemyPool()
{
	for (AEnemyBase* Enemy : EnemyPool)
	{
		if (Enemy != nullptr)
		{
			Enemy->DeactivatePoolObject();
		}
	}
}

AEnemyBase* UEnemyPoolContainer::GetInactiveEnemy()
{
	AEnemyBase* InactiveEnemy = nullptr;
	for (AEnemyBase* Enemy : EnemyPool)
	{
		if (Enemy != nullptr && !Enemy->IsPoolObjectActive())
		{
			// Found an inactive enemy
			InactiveEnemy = Enemy;
			break;
		}
	}
	if (InactiveEnemy == nullptr)
	{
		FString EnemyClassName = EnemyClass != nullptr ? EnemyClass->GetName() : "(invalid)";
		UE_LOG(LogEnemyPoolContainer, Warning, TEXT("No available enemy in the pool for class %s. Increase the pool size."), *EnemyClassName);
		InactiveEnemy = CreateAndAddEnemyToPool();
	}
	return InactiveEnemy;
}

AEnemyBase* UEnemyPoolContainer::CreateAndAddEnemyToPool()
{
	AEnemyBase* NewEnemy = nullptr;
	if (ensure(EnemyClass != nullptr))
	{
		if (UWorld* World = GetWorld())
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Name = FName(EnemyClass->GetName());
			SpawnParameters.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			//EnemySpawnParams.bNoFail = true; // Enable this to capture when SpawnActor fails
			// The failure happens in LevelActor.cpp, line 738. Enable logging in console with: Log LogSpawn VeryVerbose

			NewEnemy = World->SpawnActor<AEnemyBase>(EnemyClass.Get(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParameters);
			if (NewEnemy != nullptr)
			{
				EnemyPool.Add(NewEnemy);
			}
			else
			{
				UE_LOG(LogEnemyPoolContainer, Warning, TEXT("Failed to create enemy with class %s"), *EnemyClass->GetName());
			}
		}
	}
	return NewEnemy;
}
