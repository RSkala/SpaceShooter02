// Copyright 2024 Richard Skala

#include "EnemyBase.h"

#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"

#include "PlayerShipPawn.h"

DEFINE_LOG_CATEGORY_CLASS(AEnemyBase, LogEnemy)

namespace
{
	static const TCHAR* DefaultEnemySpriteClass = TEXT("/Game/Sprites/Enemies/SPR_Enemy_001");
}

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComp"));
	SetRootComponent(RootSceneComp);

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetupAttachment(RootComponent);

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

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	// TEST: Find the player character and set as Target
	AActor* PlayerActor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerShipPawn::StaticClass());
	if (PlayerActor != nullptr)
	{
		TargetActor = PlayerActor;
	}
}

void AEnemyBase::MoveTowardsTarget(float DeltaTime)
{
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
