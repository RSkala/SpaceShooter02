// Copyright 2024 Richard Skala

#include "PickupItemBase.h"

#include "Components/SphereComponent.h"
#include "PaperSpriteComponent.h"

#include "PlayerShipPawn.h"

APickupItemBase::APickupItemBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SetRootComponent(SphereComp);

	PaperSpriteComp = CreateDefaultSubobject<UPaperSpriteComponent>("PaperSpriteComp");
	PaperSpriteComp->SetupAttachment(RootComponent);
}

void APickupItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateMovement(DeltaTime);
}

void APickupItemBase::BeginPlay()
{
	Super::BeginPlay();

	if (SphereComp != nullptr)
	{
		SphereComp->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionOverlap);
	}

	// Get a random movement direction
	float RandomAngle = FMath::FRandRange(0.0f, 360.0f);
	float xDir = FMath::Cos(FMath::DegreesToRadians(RandomAngle));
	float zDir = FMath::Sin(FMath::DegreesToRadians(RandomAngle));
	MovementDirection = FVector(xDir, 0.0f, zDir);

	float Length = MovementDirection.Length();
	UE_LOG(LogTemp, Warning, TEXT("MovementDirection Length: %f"), Length);
}

void APickupItemBase::UpdateMovement(float DeltaTime)
{
	// Get the distance to move this frame using the movement direction
	FVector MovementAmount = MovementDirection * MovementSpeed * DeltaTime;

	// Get the new enemy position and set it
	FVector NewPickupItemPosition = GetActorLocation() + MovementAmount;
	SetActorLocation(NewPickupItemPosition);

	if (UWorld* World = GetWorld())
	{
		// Use the pickup item's position as the line trace start position
		const FVector& TraceStartPos = GetActorLocation();

		// Check agains all static objects (TODO: Use custom collision object type, e.g. Walls)
		FCollisionObjectQueryParams ObjectQueryParams(FCollisionObjectQueryParams::InitType::AllStaticObjects);

		// Distance of the LineTrace. Adjust with the movement speed and deltatime
		static const float CollisionLineTraceOffset = 2.0f;
		float CollisionDistance = MovementSpeed * DeltaTime * CollisionLineTraceOffset;

		// Check collision against walls in its movement direction
		TArray<FHitResult> HitResult;
		const FVector& TraceEndPos = TraceStartPos + FVector(MovementDirection.X, 0.0f, MovementDirection.Z).GetSafeNormal() * CollisionDistance;
		if (World->LineTraceMultiByObjectType(HitResult, TraceStartPos, TraceEndPos, ObjectQueryParams))
		{
			if (HitResult.Num() > 0)
			{
				// Reflect against the wall and change movement direction
				FVector ImpactNormal = HitResult[0].ImpactNormal;
				FVector ReflectionVector = FMath::GetReflectionVector(MovementDirection, ImpactNormal);
				MovementDirection = ReflectionVector;
			}
		}
	}
}

void APickupItemBase::OnCollisionOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	APlayerShipPawn* PlayerShipPawn = Cast<APlayerShipPawn>(OtherActor);
	if (PlayerShipPawn != nullptr)
	{
		HandlePlayerPickup();
		Destroy();
	}
}

