// Copyright 2024 Richard Skala

#include "ExplosionSpritePoolContainer.h"

#include "ExplosionBase.h"
#include "ExplosionSpriteController.h"

DEFINE_LOG_CATEGORY_STATIC(LogExplosionSpritePoolContainer, Log, All)

void UExplosionSpritePoolContainer::InitExplosionSpritePool(TSubclassOf<class AExplosionBase> InExplosionSpriteClass)
{
	ExplosionSpriteClass = InExplosionSpriteClass;
	for (int32 i = 0; i < UExplosionSpriteController::MAX_EXPLOSION_SPRITES_PER_POOL; ++i)
	{
		CreateAndAddExplosionSpriteToPool();
	}
}

void UExplosionSpritePoolContainer::ResetExplosionSpritePool()
{
	for (AExplosionBase* ExplosionSprite : ExplosionSpritePool)
	{
		if (ExplosionSprite != nullptr)
		{
			ExplosionSprite->DeactivatePoolObject();
		}
	}
}

AExplosionBase* UExplosionSpritePoolContainer::GetInactiveExplosionSprite()
{
	AExplosionBase* InactiveExplosionSprite = nullptr;
	for (AExplosionBase* ExplosionSprite : ExplosionSpritePool)
	{
		if (ExplosionSprite != nullptr && !ExplosionSprite->IsPoolObjectActive())
		{
			// Found an inactive explosion sprite
			InactiveExplosionSprite = ExplosionSprite;
			break;
		}
	}
	if (InactiveExplosionSprite == nullptr)
	{
		UE_LOG(LogExplosionSpritePoolContainer, Warning, TEXT("No available explosion sprites in the pool. Increase the pool size."));
		InactiveExplosionSprite = CreateAndAddExplosionSpriteToPool();
	}
	return InactiveExplosionSprite;
}

AExplosionBase* UExplosionSpritePoolContainer::CreateAndAddExplosionSpriteToPool()
{
	AExplosionBase* NewExplosionSprite = nullptr;
	if (ensure(ExplosionSpriteClass != nullptr))
	{
		if (UWorld* World = GetWorld())
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Name = TEXT("ExplosionSprite");
			SpawnParameters.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			NewExplosionSprite = World->SpawnActor<AExplosionBase>(ExplosionSpriteClass.Get(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParameters);
			if (NewExplosionSprite != nullptr)
			{
				ExplosionSpritePool.Add(NewExplosionSprite);
			}
			else
			{
				UE_LOG(LogExplosionSpritePoolContainer, Warning, TEXT("Failed to create explosion sprite."));
			}
		}
	}
	return NewExplosionSprite;
}
