// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerShipPawn.generated.h"

UENUM(BlueprintType)
enum class ERightStickDebugBehavior : uint8 // In UE 5.4+, enums MUST be uint8
{
	FireOnly UMETA(ToolTip = "Pressing right-stick will fire as normal"),
	FireAndLineTrace UMETA(ToolTip = "Pressing right-stick will fire AND do a line trace"),
	LineTraceOnly UMETA(ToolTip = "Pressing right-stick will ONLY do a line trace in the pressed direction")
};

UCLASS()
class SPACESHOOTER02_API APlayerShipPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerShipPawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool GetPlayerDead() const { return bPlayerDead; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void UpdateMovement(float DeltaTime);
	void UpdateGamepadAimFiring();
	void UpdateExhaust();

	void DisablePlayer();

	UFUNCTION()
	virtual void OnCollisionOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

protected:
	void KeyboardMoveTriggered(const struct FInputActionValue& InputActionValue);
	void KeyboardMoveCompleted(const struct FInputActionValue& InputActionValue);

	void GamepadMoveTriggered(const struct FInputActionValue& InputActionValue);
	void GamepadMoveCompleted(const struct FInputActionValue& InputActionValue);

	void GamepadAimTriggered(const struct FInputActionValue& InputActionValue);
	void GamepadAimCompleted(const struct FInputActionValue& InputActionValue);

	void Fire(const struct FInputActionValue& InputActionValue); // TODO: Split into KeyboardFire and GamepadFire
	void MouseFire(const struct FInputActionValue& InputActionValue);
	
	void FireProjectile(FRotator ProjectileRotation);
	void PlayShootSound();

	void KillPlayer();

	void SetMouseCursorVisiblityFromInput(APlayerController* const PlayerController, bool bCursorVisible);

protected:
	// --- Components ---

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerShipPawn|Components")
	TObjectPtr<class USphereComponent> SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerShipPawn|Components")
	TObjectPtr<class UPaperSpriteComponent> PaperSpriteComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerShipPawn|Components")
	TObjectPtr<class UNiagaraComponent> ShipExhaustParticleComp;

	// --- Camera ---
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerShipPawn|Components|Camera")
	TObjectPtr<class USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerShipPawn|Components|Camera")
	TObjectPtr<class UCameraComponent> CameraComp;

	// --- Scene Components (fire point, exhaust, etc) ---

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerShipPawn|Components|Scene")
	TObjectPtr<class USceneComponent> FirePointComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerShipPawn|Components|Scene")
	TObjectPtr<class USceneComponent> FirePointRotatorComp; // Should always be at the player ship's center

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerShipPawn|Components|Scene")
	TObjectPtr<class USceneComponent> FirePointComp1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerShipPawn|Components|Scene")
	TObjectPtr<class USceneComponent> FirePointComp2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerShipPawn|Components|Scene")
	TObjectPtr<class UPaperFlipbookComponent> ShipExhaustFlipbookComp;

	// --- Input ---
	// ==== RKS TODO: First, get general input working, then move all input into a PlayerController subclass

	// Mapping Context for Input
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerShipPawn|Input")
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	// Player ship movement via keyboard, i.e. WASD
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerShipPawn|Input")
	TObjectPtr<class UInputAction> InputActionKeyboardMove;

	// Player ship movement via gamepad, i.e. left thumbstick
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerShipPawn|Input")
	TObjectPtr<class UInputAction> InputActionGamepadMove;

	// Player Ship Aim, i.e. Right Thumbstick or Mouse
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerShipPawn|Input")
	TObjectPtr<class UInputAction> InputActionGamepadAim;

	// Player Ship Shoot, i.e. Gamepad face button or Left Mouse
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerShipPawn|Input")
	TObjectPtr<class UInputAction> InputActionFire;

	// Player Ship Dash, i.e. Gamepad right bumpter or SpaceBar
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerShipPawn|Input")
	TObjectPtr<class UInputAction> InputActionDash;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerShipPawn|Input")
	TObjectPtr<class UInputAction> InputActionMouseFire;

	// --- Movement and Aiming ---
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerShipPawn|Movement & Aiming")
	FVector2D MovementDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerShipPawn|Movement & Aiming")
	FVector2D AimingDirection;

	// --- Movement ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerShipPawn|Movement & Aiming", meta = (ClampMin = "1"))
	float MoveSpeed;

	// --- Weapons and Projectiles ---

	// Class of Projectile that will be fired
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerShipPawn|Weapons & Projectiles")
	TSubclassOf<class AProjectileBase> ProjectileClass;

	// Number of shots the player can fire per second.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerShipPawn|Weapons & Projectiles", meta = (ClampMin = "0.05", UIMin = "0.05"))
	float ProjectileShotsPerSecond = 1.0f;

	// Number of seconds between shots. Calculated with 1/ProjectileShotsPerSecond.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerShipPawn|Weapons & Projectiles")
	float FireRate;

	// Amount of time that has elapsed since the player last fired a projectile
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerShipPawn|Weapons & Projectiles", meta = (Units = "Seconds"))
	float TimeSinceLastShot;

	// --- Audio ---

	// Sound that is played when the player fires
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerShipPawn|Audio")
	TObjectPtr<class USoundBase> PlayerShootSound;

	// Sound that is played when the player dies
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerShipPawn|Audio")
	TObjectPtr<class USoundBase> PlayerDeathSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShootSoundVolume = 1.0f; // Volume of shoot sound

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShootSoundPitchAdjust = 0.0f; // + and - pitch range to add to the pitch

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ShootSoundEnabled = true;

	// --- Visual ---

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerShipPawn|Visual")
	TArray<TObjectPtr<class UPaperSprite>> PlayerShipSprites;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerShipPawn|Visual")
	TObjectPtr<class UNiagaraSystem> PlayerExplosionEffect;

	// --- Collision ---

	// Extra offset for movement collision checking
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerShipPawn|Collision")
	float CollisionLineTraceOffset = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerShipPawn|Collision|Debug")
	bool bShowMovementCollisionDebug = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerShipPawn|Collision|Debug")
	ERightStickDebugBehavior RightStickDebugBehavior = ERightStickDebugBehavior::FireOnly;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShowMouseHitDebug = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bPlayerDead = false;
};
