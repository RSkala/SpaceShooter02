// Copyright 2024 Richard Skala

#include "ProjectileBase.h"

#include "Components/SphereComponent.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectileBase::CreateSpriteComponent(const TCHAR* DefaultSpritePath)
{
	// Ensure this function is ONLY being called from the constructor (since the sprite should be a child of the collision)
	FUObjectThreadContext& ThreadContext = FUObjectThreadContext::Get();
	ensureAlwaysMsgf(
		ThreadContext.IsInConstructor > 0,
		TEXT("Function %s should only be called from a Constructor!"), ANSI_TO_TCHAR(__FUNCTION__));

	// Ensure the sprite path string is valid
	ensureAlways(DefaultSpritePath != nullptr);

	// Ensure the RootComponent is valid (should have already been created in the subclass)
	ensureAlways(RootComponent != nullptr);

	// Create the bullet sprite and disable collision on it
	ProjectileSpriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("ProjectileSpriteComp"));
	ProjectileSpriteComp->SetupAttachment(RootComponent);
	ProjectileSpriteComp->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	if (HasAnyFlags(EObjectFlags::RF_ClassDefaultObject))
	{
		// Set the default projectile sprite (for the CDO only)
		ConstructorHelpers::FObjectFinderOptional<UPaperSprite> DefaultProjectileSpriteFinder(DefaultSpritePath);
		ProjectileSpriteComp->SetSprite(DefaultProjectileSpriteFinder.Get());
	}

	//BulletSpriteComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//BulletSpriteComp->SetCollisionResponseToChannel(ECC_Destructible, ECR_Overlap);
}
