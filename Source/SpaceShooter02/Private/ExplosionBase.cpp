// Copyright 2024 Richard Skala

#include "ExplosionBase.h"

#include "Components/SceneComponent.h"
#include "PaperFlipbookComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogExplosion, Log, All)

AExplosionBase::AExplosionBase()
{
	UE_LOG(LogExplosion, Log, TEXT("AExplosionBase::AExplosionBase - %s"), *GetName());

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

void AExplosionBase::BeginPlay()
{
	Super::BeginPlay();

	if (ExplosionFlipbookComp != nullptr)
	{
		// Subscribe to the OnFinishedPlaying delegate to be notified when the flipbook animation completed. bLooping MUST be false.
		ExplosionFlipbookComp->OnFinishedPlaying.AddUniqueDynamic(this, &ThisClass::OnExplosionAnimationFinished);
	}
}

void AExplosionBase::OnExplosionAnimationFinished()
{
	//UE_LOG(LogExplosion, Log, TEXT("AExplosionBase::OnExplosionAnimationFinished - %s"), *GetName());
	
	// This explosion animation has finished. destroy it.
	Destroy();
}
