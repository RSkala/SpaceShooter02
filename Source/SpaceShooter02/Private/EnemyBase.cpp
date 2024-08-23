// Copyright 2024 Richard Skala

#include "EnemyBase.h"

#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "TimerManager.h"

#include "EnemySpawner.h"
#include "PlayerShipPawn.h"
#include "SpaceShooterGameState.h"

DEFINE_LOG_CATEGORY_CLASS(AEnemyBase, LogEnemy)

FEnemyDeathDelegateSignature AEnemyBase::OnEnemyDeath;

namespace
{
	static const TCHAR* DefaultEnemySpriteClass = TEXT("/Game/Sprites/Enemies/SPR_Enemy_001");
}

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);

	PaperSpriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperSpriteComp"));
	PaperSpriteComp->SetupAttachment(RootComponent);
	PaperSpriteComp->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	PaperSpriteComp->SetRelativeRotation(FRotator(180.0f, 0.0f, 0.0f)); // Enemy sprites are all built facing DOWN. All sprites should face UP during gameplay, so rotate 180 degrees around the Y-axis (Pitch)
	if (HasAnyFlags(EObjectFlags::RF_ClassDefaultObject))
	{
		// Set the default enemy sprite (for the CDO only)
		ConstructorHelpers::FObjectFinderOptional<UPaperSprite> DefaultEnemySpriteFinder(DefaultEnemySpriteClass);
		PaperSpriteComp->SetSprite(DefaultEnemySpriteFinder.Get());
	}
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveTowardsTarget(DeltaTime);
}

void AEnemyBase::BeginDestroy()
{
	Super::BeginDestroy();
}

void AEnemyBase::ActivatePoolObject()
{
	Super::ActivatePoolObject();

	bIsSpawning = true;
	SetActorEnableCollision(false);
	if (PaperSpriteComp != nullptr)
	{
		PaperSpriteComp->SetVisibility(false, true);
	}

	const float EnemySpawnDelayTimeSeconds = 0.75f; // TODO: Get this from the animation
	GetWorldTimerManager().SetTimer(SpawnDelayTimerHandle, this, &ThisClass::OnSpawnDelayTimerElapsed, EnemySpawnDelayTimeSeconds, false);
}

void AEnemyBase::DeactivatePoolObject()
{
	Super::DeactivatePoolObject();
	TargetActor = nullptr; // Clear the target
	bIsSpawning = false;
}

void AEnemyBase::DestroyEnemy(bool bDestroyedFromBoost /*= false*/)
{
	// Notify subscribers that an enemy died
	OnEnemyDeath.Broadcast(GetActorLocation(), EnemyExplosionEffect.Get(), EnemyDeathSound.Get(), bDestroyedFromBoost);

	// Deactivate this enemy
	DeactivatePoolObject();
}

void AEnemyBase::SetTarget(TSoftObjectPtr<AActor> InTargetActor)
{
	TargetActor = InTargetActor;
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyBase::MoveTowardsTarget(float DeltaTime)
{
	if (bIsSpawning)
	{
		// Enemy is spawning in. Do not move towards target until spawning finished.
		return;
	}

	// Do not move if the player is the target and the player is dead.
	if (APlayerShipPawn* PlayerShipPawn = Cast<APlayerShipPawn>(TargetActor.Get()))
	{
		if (PlayerShipPawn->GetPlayerDead())
		{
			DeactivatePoolObject();
			return;
		}
	}

	// Get this enemy's movement direction depending on whether or not it has a target
	FVector MovementDirection;
	if (TargetActor == nullptr)
	{
		// This enemy has no target. Move directly in its "up" direction
		MovementDirection = GetActorUpVector();
	}
	else
	{
		// This enemy has a target. Move toward the target.
		FVector TargetPosition = TargetActor->GetActorLocation();

		// Get the normalized vector from this enemy to the target (A->B = B-A)
		MovementDirection = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	}

	// Move enemy in the movement direction

	// Get this enemy's current position
	FVector EnemyPosition = GetActorLocation();

	// Get the distance to move this frame using the movement direction
	//FVector2D MovementAmount2D = MovementDirection * MoveSpeed * DeltaTime;
	FVector MovementAmount = MovementDirection * MoveSpeed * DeltaTime;

	// Get the new enemy position and set it
	FVector NewEnemyPosition = EnemyPosition + MovementAmount;
	SetActorLocation(NewEnemyPosition);

	// Rotate this enemy towards its target (if it has one)
	if(bRotateToFaceTarget && TargetActor != nullptr)
	{
		// Get the angle from the world-up vector to the movement direction
		float Dot = FVector::UnitZ().Dot(MovementDirection);
		float Acos = FMath::Acos(Dot);
		float AngleDegrees = FMath::RadiansToDegrees(Acos);

		// Use the cross product to determine which way to rotate (i.e. Clockwise or Counterclockwise)
		FVector Cross = FVector::CrossProduct(FVector::UnitZ(), MovementDirection);
		float AngleDirectionMultipler = Cross.Y >= 0.0f ? -1.0f : 1.0f;
		AngleDegrees *= AngleDirectionMultipler;

		// Set the new enemy rotation
		FRotator NewPlayerShipRotation = UKismetMathLibrary::MakeRotator(0.0f, AngleDegrees, 0.0f);
		SetActorRotation(NewPlayerShipRotation);
	}
}

void AEnemyBase::OnSpawnDelayTimerElapsed()
{
	if (SpawnDelayTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(SpawnDelayTimerHandle);
	}

	bIsSpawning = false;
	SetActorEnableCollision(true);
	if (PaperSpriteComp != nullptr)
	{
		PaperSpriteComp->SetVisibility(true, true);
	}
}
