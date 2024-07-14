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

	virtual TSubclassOf<class UShapeComponent> GetCollisionVolumeComponentClass() const override;
	virtual const TCHAR* GetDefaultSpritePath() const override;

	virtual void OnCollisionOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;
};
