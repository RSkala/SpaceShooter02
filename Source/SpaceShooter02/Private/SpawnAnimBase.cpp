// Copyright 2024 Richard Skala

#include "SpawnAnimBase.h"

#include "PaperFlipbookComponent.h"

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

void ASpawnAnimBase::BeginPlay()
{
	Super::BeginPlay();

	if (SpawnAnimFlipbookComp != nullptr)
	{
		// Subscribe to the OnFinishedPlaying delegate to be notified when the flipbook animation completed. bLooping MUST be false.
		SpawnAnimFlipbookComp->OnFinishedPlaying.AddUniqueDynamic(this, &ThisClass::OnSpawnAnimationFinished);
	}
}

void ASpawnAnimBase::OnSpawnAnimationFinished()
{
	// This spawn animation has finished. Destroy it.
	Destroy();
}
