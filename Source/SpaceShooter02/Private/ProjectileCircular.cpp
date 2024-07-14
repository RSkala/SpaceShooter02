// Copyright 2024 Richard Skala

#include "ProjectileCircular.h"

#include "Components/SphereComponent.h"

namespace
{
	static const TCHAR* DefaultProjectileSpritePath = TEXT("/Game/Sprites/Projectiles/SPR_Projectile_02");
}

AProjectileCircular::AProjectileCircular()
{
	CreateProjectileDefaultSubobjects();
}

void AProjectileCircular::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileCircular::BeginPlay()
{
	Super::BeginPlay();
}

TSubclassOf<UShapeComponent> AProjectileCircular::GetCollisionVolumeComponentClass() const
{
	return USphereComponent::StaticClass();
}

const TCHAR* AProjectileCircular::GetDefaultSpritePath() const
{
	return DefaultProjectileSpritePath;
}
