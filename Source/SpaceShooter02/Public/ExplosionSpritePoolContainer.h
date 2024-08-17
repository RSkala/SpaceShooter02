// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ExplosionSpritePoolContainer.generated.h"

// This class is a workaround because Unreal does not support nested TArrays, e.g. TArray<TArray<AExplosionBase>> (at least for UPROPERTY)
// I originally tried using a USTRUCT(), but I was having weird garbage collection issues
// https://forums.unrealengine.com/t/tarray-of-tarray-support-request/357202/2
UCLASS(Blueprintable)
class SPACESHOOTER02_API UExplosionSpritePoolContainer : public UObject
{
	GENERATED_BODY()

public:
	void InitExplosionSpritePool(TSubclassOf<class AExplosionBase> InExplosionSpriteClass);
	void ResetExplosionSpritePool();
	AExplosionBase* GetInactiveExplosionSprite();

private:
	AExplosionBase* CreateAndAddExplosionSpriteToPool();
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<class AExplosionBase>> ExplosionSpritePool;

	UPROPERTY()
	TSoftClassPtr<class AExplosionBase> ExplosionSpriteClass;
};
