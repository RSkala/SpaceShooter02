// Copyright 2024 Richard Skala

#include "ProjectileRectangular.h"

#include "Components/BoxComponent.h"

namespace
{
	static const TCHAR* DefaultProjectileSpritePath = TEXT("/Game/Sprites/Projectiles/SPR_Projectile_01");
}

AProjectileRectangular::AProjectileRectangular()
{
	CreateCollisionVolume();
	CreateSpriteComponent(DefaultProjectileSpritePath);
}

void AProjectileRectangular::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileRectangular::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectileRectangular::CreateCollisionVolume()
{
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
}
