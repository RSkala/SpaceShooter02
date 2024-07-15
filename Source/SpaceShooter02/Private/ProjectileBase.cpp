// Copyright 2024 Richard Skala

#include "ProjectileBase.h"

#include "Components/ShapeComponent.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"

DEFINE_LOG_CATEGORY_CLASS(AProjectileBase, LogProjectiles)

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateMovement(DeltaTime);
	UpdateLifetime(DeltaTime);
}

void AProjectileBase::Init(FVector ProjectilePosition, FRotator ProjectileRotation)
{
	// TODO: Set movespeed, etc

	//SetActorLocation(ProjectilePosition);
	//SetActorRotation(ProjectileRotation);
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (CollisionShapeComp != nullptr)
	{
		CollisionShapeComp->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionOverlap);
	}	
}

void AProjectileBase::CreateProjectileDefaultSubobjects()
{
	// Create the scene root component
	RootSceneComp = CreateDefaultSubobject<USceneComponent>("RootSceneComp");
	SetRootComponent(RootSceneComp);

	// Create the collision component
	CreateCollisionVolumeComponent(GetCollisionVolumeComponentClass());

	// Create the sprite component
	CreateSpriteComponent(GetDefaultSpritePath());
}

void AProjectileBase::OnCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogProjectiles, Log, TEXT("AProjectileBase::OnCollisionOverlap - %s"), *GetName());
}

void AProjectileBase::CreateCollisionVolumeComponent(TSubclassOf<UShapeComponent> CollisionVolumeClass)
{
	// Ensure this function is ONLY being called from the constructor
	EnsureConstructorOnlyCall(GET_FUNCTION_NAME_CHECKED(AProjectileBase, CreateCollisionVolumeComponent));

	// Ensure the RootComponent is valid (should have already been created before this function call)
	ensureAlways(RootComponent != nullptr);

	// Create the collision shape using the given collision volume class and attach to the root
	CollisionShapeComp = Cast<UShapeComponent>(CreateDefaultSubobject(TEXT("CollisionShapeComp"), CollisionVolumeClass, CollisionVolumeClass, true, false));
	CollisionShapeComp->SetupAttachment(RootComponent);
}

void AProjectileBase::CreateSpriteComponent(const TCHAR* DefaultSpritePath)
{
	// Ensure this function is ONLY being called from the constructor
	EnsureConstructorOnlyCall(GET_FUNCTION_NAME_CHECKED(AProjectileBase, CreateSpriteComponent));

	// Ensure the sprite path string is valid
	ensureAlways(DefaultSpritePath != nullptr);

	// Ensure the RootComponent is valid (should have already been created before this function call)
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

void AProjectileBase::EnsureConstructorOnlyCall(FName CallingFunctionName)
{
	// Ensure the calling function is only called in a constructor
	FUObjectThreadContext& ThreadContext = FUObjectThreadContext::Get();
	ensureAlwaysMsgf(
		ThreadContext.IsInConstructor > 0,
		TEXT("Function %s should only be called from a Constructor!"), *CallingFunctionName.ToString());
}

TSubclassOf<UShapeComponent> AProjectileBase::GetCollisionVolumeComponentClass() const
{
	checkf(false, TEXT("AProjectileBase::GetCollisionVolumeComponentClass MUST be overidden in a subclass"));
	return UShapeComponent::StaticClass();
}

const TCHAR* AProjectileBase::GetDefaultSpritePath() const
{
	checkf(false, TEXT("AProjectileBase::GetDefaultSpritePath MUST be overidden in a subclass"));
	return L"";
}

void AProjectileBase::UpdateMovement(float DeltaTime)
{
	// By default, move the projectile in one direction (consider the +Z or "Up" the projectile "forward" direction)

	// Get the projectile's current position
	FVector ProjectilePosition = GetActorLocation();

	// Get the distance to move this frame using the projectiles "up" direction
	FTransform ProjectileTransform = GetActorTransform();
	
	// Get this projectile's "up" direction for the movement direction
	FVector ProjectileUp = GetActorUpVector();
	FVector MovementDirection = ProjectileUp;
	FVector MovementAmount = MovementDirection * MoveSpeed * DeltaTime;

	FVector NewProjectilePosition = ProjectilePosition + MovementAmount;
	SetActorLocation(NewProjectilePosition);
}

void AProjectileBase::UpdateLifetime(float DeltaTime)
{
	TimeAlive += DeltaTime;
	if (TimeAlive >= LifetimeSeconds)
	{
		// This projectile has exceeded its lifespawn. Destroy this projectile.
		Destroy();
	}
}



