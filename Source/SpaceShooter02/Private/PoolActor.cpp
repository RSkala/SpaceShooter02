// Copyright 2024 Richard Skala

#include "PoolActor.h"

const FVector APoolActor::InactivePoolActorPosition = FVector(-10000.0f, -10000.0f, -10000.0f);

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
	SetActorEnableCollision(true); // Enable collision components
	SetActorTickEnabled(true); // Start ticking
	TimeAlive = 0.0f;
}

void APoolActor::DeactivatePoolObject()
{
	bIsPoolObjectActive = false;
	SetActorHiddenInGame(true); // Hide visible components
	SetActorEnableCollision(false); // Disable collision components
	SetActorTickEnabled(false); // Stop ticking
	SetActorLocation(InactivePoolActorPosition); // Move actor to somewhere far outside game bounds
	TimeAlive = 0.0f;
}

void APoolActor::BeginPlay()
{
	Super::BeginPlay();

	// Always start pooled actors deactivated
	DeactivatePoolObject();
}

void APoolActor::UpdateLifetime(float DeltaTime)
{
	TimeAlive += DeltaTime;
	if (TimeAlive >= LifeTimeSeconds)
	{
		DeactivatePoolObject();
	}
}



