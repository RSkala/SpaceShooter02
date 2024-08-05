// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupItemBase.generated.h"

UCLASS(Abstract, NotBlueprintable)
class SPACESHOOTER02_API APickupItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	APickupItemBase();
	virtual void Tick(float DeltaTime) override;

	void SetMovementDirection(FVector InMovementDirection) { MovementDirection = InMovementDirection; }

protected:
	virtual void BeginPlay() override;

	virtual void UpdateMovement(float DeltaTime);
	virtual void HandlePlayerPickup() {};

	UFUNCTION()
	virtual void OnCollisionOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

protected:
	// --- Components ---

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USphereComponent> SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UPaperSpriteComponent> PaperSpriteComp;

	// --- Movement ---

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector MovementDirection;

	// --- Lifetime ---

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LifeTimeSeconds = 3.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float TimeAlive = 0.0f;
};
