// Copyright 2024 Richard Skala

#include "SpawnAnimBase.h"

#include "PaperFlipbookComponent.h"

const FVector ASpawnAnimBase::InactivePosition = FVector(-9000.0f, -9000.0f, 9000.0f);

ASpawnAnimBase::ASpawnAnimBase()
{
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComp = CreateDefaultSubobject<USceneComponent>("RootSceneComp");
	SetRootComponent(RootSceneComp);

	SpawnAnimFlipbookComp = CreateDefaultSubobject<UPaperFlipbookComponent>("SpawnAnimFlipbookComp");
	SpawnAnimFlipbookComp->SetupAttachment(RootSceneComp);
	SpawnAnimFlipbookComp->SetLooping(false); // Spawn animation should not loop
	SpawnAnimFlipbookComp->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

void ASpawnAnimBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpawnAnimBase::ActivatePoolObject()
{
	Super::ActivatePoolObject();
	if (SpawnAnimFlipbookComp != nullptr)
	{
		SpawnAnimFlipbookComp->SetPlaybackPosition(0.0f, false);
		SpawnAnimFlipbookComp->PlayFromStart();
	}
}

void ASpawnAnimBase::DeactivatePoolObject()
{
	Super::DeactivatePoolObject();
	if (SpawnAnimFlipbookComp != nullptr)
	{
		SpawnAnimFlipbookComp->Stop();
	}
}

void ASpawnAnimBase::BeginPlay()
{
	Super::BeginPlay();

	if (SpawnAnimFlipbookComp != nullptr)
	{
		// Subscribe to the OnFinishedPlaying delegate to be notified when the flipbook animation completed. bLooping MUST be false.
		SpawnAnimFlipbookComp->OnFinishedPlaying.AddUniqueDynamic(this, &ThisClass::OnSpawnAnimationFinished);
	}
}

FVector ASpawnAnimBase::GetInactivePoolObjectPosition() const
{
	return InactivePosition;
}

void ASpawnAnimBase::OnSpawnAnimationFinished()
{
	// This spawn animation has finished. Deactivate it.
	DeactivatePoolObject();
}
