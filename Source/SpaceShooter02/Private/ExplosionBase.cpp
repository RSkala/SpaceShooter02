// Copyright 2024 Richard Skala

#include "ExplosionBase.h"

#include "Components/SceneComponent.h"
#include "PaperFlipbookComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogExplosion, Log, All)

const FVector AExplosionBase::InactivePosition = FVector(9000.0f, -9000.0f, -9000.0f);

AExplosionBase::AExplosionBase()
{
	//UE_LOG(LogExplosion, Log, TEXT("AExplosionBase::AExplosionBase - %s"), *GetName());

	PrimaryActorTick.bCanEverTick = true;

	RootSceneComp = CreateDefaultSubobject<USceneComponent>("RootSceneComp");
	SetRootComponent(RootSceneComp);

	ExplosionFlipbookComp = CreateDefaultSubobject<UPaperFlipbookComponent>("ExplosionFlipbookComp");
	ExplosionFlipbookComp->SetupAttachment(RootSceneComp);
	ExplosionFlipbookComp->SetLooping(false); // Explosions should not loop
	ExplosionFlipbookComp->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

void AExplosionBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AExplosionBase::BeginDestroy()
{
	Super::BeginDestroy();
}

void AExplosionBase::ActivatePoolObject()
{
	Super::ActivatePoolObject();
	if (ExplosionFlipbookComp != nullptr)
	{
		ExplosionFlipbookComp->PlayFromStart();
	}
}

void AExplosionBase::DeactivatePoolObject()
{
	Super::DeactivatePoolObject();
	if (ExplosionFlipbookComp != nullptr)
	{
		ExplosionFlipbookComp->Stop();
	}
}

void AExplosionBase::BeginPlay()
{
	Super::BeginPlay();

	if (ExplosionFlipbookComp != nullptr)
	{
		// Subscribe to the OnFinishedPlaying delegate to be notified when the flipbook animation completed. bLooping MUST be false.
		ExplosionFlipbookComp->OnFinishedPlaying.AddUniqueDynamic(this, &ThisClass::OnExplosionAnimationFinished);
	}
}

FVector AExplosionBase::GetInactivePoolObjectPosition() const
{
	return InactivePosition;
}

void AExplosionBase::OnExplosionAnimationFinished()
{
	//UE_LOG(LogExplosion, Log, TEXT("AExplosionBase::OnExplosionAnimationFinished - %s"), *GetName());
	
	// This explosion animation has finished. Deactivate it.
	DeactivatePoolObject();
}
