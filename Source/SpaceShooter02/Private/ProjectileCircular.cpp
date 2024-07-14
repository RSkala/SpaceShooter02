// Copyright 2024 Richard Skala

#include "ProjectileCircular.h"

#include "Components/SphereComponent.h"

namespace
{
	static const TCHAR* DefaultProjectileSpritePath = TEXT("/Game/Sprites/Projectiles/SPR_Projectile_01");
}

AProjectileCircular::AProjectileCircular()
{
	CreateCollisionVolume();
	CreateSpriteComponent(DefaultProjectileSpritePath);
}

void AProjectileCircular::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileCircular::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectileCircular::CreateCollisionVolume()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);
}
