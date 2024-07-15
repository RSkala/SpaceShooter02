// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

UCLASS(Abstract, NotBlueprintable)
class SPACESHOOTER02_API AProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	DECLARE_LOG_CATEGORY_CLASS(LogProjectiles, Log, All)
	
public:	
	AProjectileBase();
	virtual void Tick(float DeltaTime) override;
	virtual void Init(FVector ProjectilePosition, FRotator ProjectileRotation);

protected:
	virtual void BeginPlay() override;

	void CreateProjectileDefaultSubobjects(); // Should be called in the constructor of any subclass. Will create all the proper default subobjects.
	virtual TSubclassOf<class UShapeComponent> GetCollisionVolumeComponentClass() const; // PURE_VIRTUAL(GetCollisionVolumeComponentClass, ;)
	virtual const TCHAR* GetDefaultSpritePath() const; // PURE_VIRTUAL(GetDefaultSpritePath, ;)

	virtual void UpdateMovement(float DeltaTime);
	virtual void UpdateLifetime(float DeltaTime);

	UFUNCTION()
	virtual void OnCollisionOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

private:
	void CreateCollisionVolumeComponent(TSubclassOf<UShapeComponent> CollisionVolumeClass);
	void CreateSpriteComponent(const TCHAR* DefaultSpritePath);
	void EnsureConstructorOnlyCall(FName CallingFunctionName);

protected:

	// --- Components ---

	// Root scene component that all subobjects should be attached to. This way, we can scale the collision without affecting the sprite.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USceneComponent> RootSceneComp;

	// Collision Volume. This is expected to be created in a subclass with its own specific type (e.g. Sphere, Box, etc)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UShapeComponent> CollisionShapeComp;

	// Sprite for projectile
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UPaperSpriteComponent> ProjectileSpriteComp;

	// --- Projectile Behavior ---

	// How quickly this projectile moves across the screen
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileBase|Behavior", meta = (ClampMin = "0"))
	float MoveSpeed = 2000.0f;

	// The amount of damage this projectile deals to a valid target
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileBase|Behavior", meta = (ClampMin = "0"))
	float Damage = 100.0f;

	// How long this projectile stays alive before being destroyed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileBase|Behavior", meta = (ClampMin = "0"))
	float LifetimeSeconds = 10.0f;

	// How long this projectile has been active
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ProjectileBase|Behavior")
	float TimeAlive = 0.0f;
};
