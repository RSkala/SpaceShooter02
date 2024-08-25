// Copyright 2024 Richard Skala

#include "PoolActor.h"

APoolActor::APoolActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APoolActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateLifetime(DeltaTime);
}

void APoolActor::ActivatePoolObject()
{
	bIsPoolObjectActive = true;
	SetActorHiddenInGame(false); // Show visible components
	if (EnableCollisionOnActivate())
	{
		SetActorEnableCollision(true); // Enable collision components
	}
	SetActorTickEnabled(true); // Start ticking
	TimeAlive = 0.0f;
}

void APoolActor::DeactivatePoolObject()
{
	bIsPoolObjectActive = false;
	SetActorHiddenInGame(true); // Hide visible components
	SetActorEnableCollision(false); // Disable collision components
	SetActorTickEnabled(false); // Stop ticking
	SetActorLocation(GetInactivePoolObjectPosition()); // Move actor to somewhere far outside game bounds
	TimeAlive = 0.0f;
}

void APoolActor::BeginPlay()
{
	Super::BeginPlay();

	// Always start pooled actors deactivated
	DeactivatePoolObject();
}

FVector APoolActor::GetInactivePoolObjectPosition() const
{
	checkf(false, TEXT("APoolActor::GetInactivePoolObjectPosition MUST be overidden in a subclass"));
	return FVector();
}

void APoolActor::UpdateLifetime(float DeltaTime)
{
	TimeAlive += DeltaTime;
	if (TimeAlive >= LifeTimeSeconds)
	{
		DeactivatePoolObject();
	}
}



