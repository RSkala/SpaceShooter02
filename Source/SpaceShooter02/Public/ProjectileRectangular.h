// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "ProjectileRectangular.generated.h"

// Projectile that is expected to have rectangular collision (box)
UCLASS(Blueprintable)
class SPACESHOOTER02_API AProjectileRectangular : public AProjectileBase
{
	GENERATED_BODY()

public:
	AProjectileRectangular();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void CreateCollisionVolume() override;

protected:
	// Box Component - Used for collisions
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UBoxComponent> BoxComp;
};
