// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"

#include "PoolActor.h"

#include "PickupItemBase.generated.h"

UCLASS(Abstract, NotBlueprintable)
class SPACESHOOTER02_API APickupItemBase : public APoolActor
{
	GENERATED_BODY()
	
public:	
	APickupItemBase();
	virtual void Tick(float DeltaTime) override;
	virtual void ActivatePoolObject() override;
	virtual void DeactivatePoolObject() override;

	void SetMovementDirection(FVector InMovementDirection) { MovementDirection = InMovementDirection; }

protected:
	virtual void BeginPlay() override;
	virtual FVector GetInactivePoolObjectPosition() const override;
	virtual void UpdateLifetime(float DeltaTime) override;

	virtual void UpdateMovement(float DeltaTime);
	virtual void UpdateAttractionMovement(float DeltaTime);
	virtual void UpdateNonAttractionMovement(float DeltaTime);
	virtual void UpdateTargetAttraction(float DeltaTime);

	virtual bool IsAttractingToTarget() const { return AttractionTargetActor != nullptr; }

	virtual void HandlePlayerPickup() {};

	UFUNCTION()
	virtual void OnCollisionOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnPlayerShipDestroyed();

protected:
	// --- Components ---

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USphereComponent> SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UPaperSpriteComponent> PaperSpriteComp;

	// --- Movement ---

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed = 200.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector MovementDirection;

	// --- Target Attraction ---

	// If enabled, will be "pulled" towards the target that is "picking up" this item
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseTargetAttraction = true;

	// Target actor to be "pulled" towards
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TObjectPtr<AActor> AttractionTargetActor;

	// If the player is this close to a pickup item, start attraction
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TargetAttractDistance = 200.0f;

	// How quickly this pickup item moves towards its attraction target (if it has one)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttractionMovementSpeed = 1750.0f;

	// Used for easily getting the player.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class APlayerShipPawn> PlayerShipPawn;

	static const FVector InactivePosition;
};
