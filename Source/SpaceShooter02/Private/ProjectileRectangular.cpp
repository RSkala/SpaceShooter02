// Copyright 2024 Richard Skala

#include "ProjectileRectangular.h"

#include "Components/BoxComponent.h"

#include "EnemyBase.h"

namespace ProjectileRectangular
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
	return ProjectileRectangular::DefaultProjectileSpritePath;
}

void AProjectileRectangular::OnCollisionOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	// Ignore this collision if the colliding actor is the Instigator (i.e. the Actor that "fired" this projectile)
	if (OtherActor != nullptr && OtherActor == GetInstigator())
	{
		return;
	}

	if (AEnemyBase* CollidingEnemy = Cast<AEnemyBase>(OtherActor))
	{
		// The colliding actor is an enemy. Deal damage / destroy the enemy.
		//CollidingEnemy->Destroy();
		CollidingEnemy->DestroyEnemy();
	}
}
