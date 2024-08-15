// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ProjectileController.generated.h"

UCLASS(Abstract, Blueprintable)
class SPACESHOOTER02_API UProjectileController : public UObject
{
	GENERATED_BODY()

public:
	void InitProjectilePool();
	void ResetProjectilePool();
	class AProjectileBase* GetInactiveProjectile();

private:
	class AProjectileBase* CreateAndAddNewProjectile();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<class AProjectileBase> ProjectileClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<class AProjectileBase>> ProjectilePool;

private:
	static constexpr int32 MAX_PROJECTILES = 500;
};
