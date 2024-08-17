// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ExplosionSpriteController.generated.h"

UCLASS(Blueprintable)
class SPACESHOOTER02_API UExplosionSpriteController : public UObject
{
	GENERATED_BODY()

public:
	virtual void BeginDestroy() override;

	void InitExplosionSpritePools();
	void ResetExplosionSpritePools();
	class AExplosionBase* GetRandomInactiveExplosionSprite();

public:
	static constexpr int32 MAX_EXPLOSION_SPRITES_PER_POOL = 50;

private:
	// List of explosion sprite classes to create pools from
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<class AExplosionBase>> ExplosionSpriteClasses;

	// List of explosion sprite pools. Number of pools will be exactly the number of explosion sprite classes.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<class UExplosionSpritePoolContainer>> ExplosionSpritePoolContainers;
};
