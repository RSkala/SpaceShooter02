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
	AProjectileBase();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void CreateCollisionVolume() { checkf(false, TEXT("AProjectileBase::CreateCollisionVolume MUST be overidden in a subclass")); } //PURE_VIRTUAL(CreateCollisionVolume, ;);
	void CreateSpriteComponent(const TCHAR* DefaultSpritePath);

protected:

	// --- Components ---

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
};
