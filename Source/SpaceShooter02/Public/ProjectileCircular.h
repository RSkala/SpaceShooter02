// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "ProjectileCircular.generated.h"

// Projectile that is expected to have circular collision (sphere)
UCLASS(Blueprintable)
class SPACESHOOTER02_API AProjectileCircular : public AProjectileBase
{
	GENERATED_BODY()
	
public:	
	AProjectileCircular();
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
