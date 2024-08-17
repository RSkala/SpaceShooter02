// Copyright 2024 Richard Skala

#include "ExplosionSpriteController.h"

#include "ExplosionBase.h"
#include "ExplosionSpritePoolContainer.h"

DEFINE_LOG_CATEGORY_STATIC(LogExplosionSpriteController, Log, All)

void UExplosionSpriteController::BeginDestroy()
{
	Super::BeginDestroy();
}

void UExplosionSpriteController::InitExplosionSpritePools()
{
	if (ExplosionSpriteClasses.Num() <= 0)
	{
		return;
	}

	for (TSubclassOf<AExplosionBase> ExplosionSpriteClass : ExplosionSpriteClasses)
	{
		if (ensureAlways(ExplosionSpriteClass != nullptr))
		{
			// Create a new pool
			UExplosionSpritePoolContainer* ExplosionSpritePoolContainer = NewObject<UExplosionSpritePoolContainer>(this, UExplosionSpritePoolContainer::StaticClass());
			if (ExplosionSpritePoolContainer != nullptr)
			{
				ExplosionSpritePoolContainers.Add(ExplosionSpritePoolContainer);
				ExplosionSpritePoolContainer->InitExplosionSpritePool(ExplosionSpriteClass);
			}
		}
	}
}

void UExplosionSpriteController::ResetExplosionSpritePools()
{
	for (UExplosionSpritePoolContainer* ExplosionSpritePoolContainer : ExplosionSpritePoolContainers)
	{
		if (ExplosionSpritePoolContainer != nullptr)
		{
			ExplosionSpritePoolContainer->ResetExplosionSpritePool();
		}
	}
}

AExplosionBase* UExplosionSpriteController::GetRandomInactiveExplosionSprite()
{
	AExplosionBase* ExplosionSprite = nullptr;
	int32 RandomIndex = FMath::RandRange(0, ExplosionSpritePoolContainers.Num() - 1);
	UExplosionSpritePoolContainer* PoolContainer = ExplosionSpritePoolContainers[RandomIndex];
	if (PoolContainer != nullptr)
	{
		ExplosionSprite = PoolContainer->GetInactiveExplosionSprite();
	}
	return ExplosionSprite;
}
