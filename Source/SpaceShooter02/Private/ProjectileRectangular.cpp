// Copyright 2024 Richard Skala

#include "ProjectileRectangular.h"

#include "Components/BoxComponent.h"

namespace
{
	static const TCHAR* DefaultProjectileSpritePath = TEXT("/Game/Sprites/Projectiles/SPR_Projectile_01");
}

AProjectileRectangular::AProjectileRectangular()
{
	CreateProjectileDefaultSubobjects();
}

void AProjectileRectangular::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileRectangular::BeginPlay()
{
	Super::BeginPlay();
}

TSubclassOf<UShapeComponent> AProjectileRectangular::GetCollisionVolumeComponentClass() const
{
	return UBoxComponent::StaticClass();
}

const TCHAR* AProjectileRectangular::GetDefaultSpritePath() const
{
	return DefaultProjectileSpritePath;
}

void AProjectileRectangular::OnCollisionOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	UE_LOG(LogProjectiles, Log, TEXT("AProjectileRectangular::OnCollisionOverlap - %s"), *GetName());
}
