// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerShipPawn.generated.h"

// Delegate for when the player ship spawns in
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerShipSpawnedDelegateSignature, APlayerShipPawn*, PlayerShipPawn);

// Delegate for when the player ship is destroyed (i.e. Game Over)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerShipDestroyedDelegateSignature);

// Delegate for updating the player's powerup weapon meter
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerPowerupTimerUpdatedDelegateSignature, float, Percent);

// Delegate for updating the player's dash meter
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerDashUpdatedDelegateSignature, float, Percent);


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

	void EnablePlayer();

	bool IsPlayerDisabled() const { return IsHidden(); }

	static FPlayerShipSpawnedDelegateSignature OnPlayerShipSpawned;
	static FPlayerShipDestroyedDelegateSignature OnPlayerShipDestroyed;
	static FPlayerPowerupTimerUpdatedDelegateSignature OnPlayerPowerupTimerUpdated;
	static FPlayerDashUpdatedDelegateSignature OnPlayerDashUpdated;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void UpdateMovement(float DeltaTime);
	void UpdateGamepadAimFiring();
	void UpdateExhaust();

	void DisablePlayer();
	//void EnablePlayer();

	UFUNCTION()
	virtual void OnCollisionOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnGameStarted();

	UFUNCTION()
	void OnGameEnded();

	UFUNCTION()
	void OnPlayerShipSelected(class UPaperSprite* SelectedShipSprite);

protected:
	void KeyboardMoveTriggered(const struct FInputActionValue& InputActionValue);
	void KeyboardMoveCompleted(const struct FInputActionValue& InputActionValue);

	void GamepadMoveTriggered(const struct FInputActionValue& InputActionValue);
	void GamepadMoveCompleted(const struct FInputActionValue& InputActionValue);

	void GamepadAimTriggered(const struct FInputActionValue& InputActionValue);
	void GamepadAimCompleted(const struct FInputActionValue& InputActionValue);

	void Fire(const struct FInputActionValue& InputActionValue); // TODO: Split into KeyboardFire and GamepadFire
	void MouseFire(const struct FInputActionValue& InputActionValue);

	void InputDash(const struct FInputActionValue& InputActionValue);
	
	void FireProjectile(FRotator ProjectileRotation);
	void PlayShootSound();

	void KillPlayer();

	void SetMouseCursorVisiblityFromInput(APlayerController* const PlayerController, bool bCursorVisible);

	void UpdateSatelliteWeaponRotation(float DeltaTime);
	void UpdateSatelliteWeaponAimRotation(FRotator AimRotation);
	void UpdateSatelliteWeaponAimRotation(FRotator AimRotation, class UPaperSpriteComponent* const SatelliteWeapon);

	void FireProjectileFromSatelliteWeapon(UWorld* const World, const FActorSpawnParameters& ProjectileSpawnParameters);
	void FireProjectileFromSatelliteWeapon(
		UWorld* const World,
		class UPaperSpriteComponent* const SatelliteWeapon,
		const FActorSpawnParameters& ProjectileSpawnParameters);

	void DisableSatelliteWeapons();
	void DisableSatelliteWeapon(class UPaperSpriteComponent* const SatelliteWeapon);
	void EnableSatelliteWeapon(class UPaperSpriteComponent* const SatelliteWeapon);

	UFUNCTION(Exec)
	void AddSatelliteWeapon();

	UFUNCTION(Exec)
	void RemoveSatelliteWeapon();

	bool PlayerHasPowerup() const;
	void PowerupTimerElapsed();

	UFUNCTION()
	void OnScoreMultiplierPickedUp(int32 ScoreMultiplierValue);

	UFUNCTION()
	void PickupItemPercentChanged(float Percent);

	void ShowDashShield();
	void HideDashShield();

protected:
	// --- Components ---

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerShipPawn|Components")
	TObjectPtr<class USphereComponent> SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerShipPawn|Components")
	TObjectPtr<class UPaperSpriteComponent> PaperSpriteComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerShipPawn|Components")
	TObjectPtr<class UNiagaraComponent> ShipExhaustParticleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerShipPawn|Components")
	TObjectPtr<class UPaperSpriteComponent> DashShieldSpriteComp;

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

	// Rotates at a certain rate. Used for rotating a satellite weapon around the player ship.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerShipPawn|Components")
	TObjectPtr<class USceneComponent> SatelliteWeaponRotatorComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerShipPawn|Components")
	TObjectPtr<class UPaperSpriteComponent> SatelliteWeaponSprite1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerShipPawn|Components")
	TObjectPtr<class UPaperSpriteComponent> SatelliteWeaponSprite2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerShipPawn|Components")
	TObjectPtr<class UPaperSpriteComponent> SatelliteWeaponSprite3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerShipPawn|Components")
	TObjectPtr<class UPaperSpriteComponent> SatelliteWeaponSprite4;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerShipPawn|Components")
	TObjectPtr<class USceneComponent> PowerupEffectAttachPoint;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerShipPawn|Movement & Aiming", meta = (ClampMin = "1"))
	float DashSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerShipPawn|Movement & Aiming", meta = (ClampMin = "1"))
	float DashTimeElapsed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerShipPawn|Movement & Aiming")
	bool bIsDashing = false;

	// How long it takes to recharge Dash ability
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerShipPawn|Movement & Aiming")
	float DashRechargeTime = 10.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerShipPawn|Movement & Aiming")
	float DashRechargeTimeElapsed = 0.0f;

	// Enlarge the collision sphere while dashing so the player can "crash" into more enemies
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerShipPawn|Movement & Aiming")
	float DashCollisionSphereRadius = 100.0f;

	// Save the collision sphere radius before dashing then restore after dashing
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerShipPawn|Movement & Aiming")
	float PreDashCollisionSphereRadius;

	// How long (in seconds) the player stays in "Dash" mode
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerShipPawn|Movement & Aiming")
	float DashTime = 0.5f;

	// Amount to multiply to the move speed while dashing
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerShipPawn|Movement & Aiming")
	float DashMoveSpeedMultiplier = 2.0f;

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

	// How quickly the satellite weapon rotates around the player ship. In degrees per second.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerShipPawn|Weapons & Projectiles")
	float SatelliteWeaponRotationSpeed = 360.0f;

	// Debug value to see the current rotation in the Details panel
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerShipPawn|Weapons & Projectiles")
	float CurrentSatelliteWeaponRotationAngle = 0.0f;

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
	bool bShootSoundEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bPlayShootSoundDuringPowerup = true;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bPlayerInvincible = false;

	// How long a powerup stays "active" for before being removed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.1", UIMin = "0.1"))
	float PowerupActiveTime = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float PowerupActiveTimer = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class USoundBase> PowerupEarnedSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<class USoundBase>> ShipDashSounds;

	// Played when adding time to current powerup
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UNiagaraSystem> PlayerPowerupEffectSmall;

	// Played when activating powerup
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UNiagaraSystem> PlayerPowerupEffectLarge;

	// When "powerup" is active, number of seconds to add to the active powerup timer
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SecondsToAddForActivePowerup = 0.5f;

	// -------------------------------------------------------------------------------------
	// Below stuff copied from SpaceShooterGameState. Probably should really be in a PlayerState.

	// Total multipliers collected during game. Currently unused. May use for savegames/leaderboards, etc.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 TotalMultipliersCollected = 0;

	// Total multipliers collected for powerup
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 NumMultipliersCollectedForPowerup = 0;

	// Number of multiplier pickups needed to be collected to "activate" powerup
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 NumMultipliersNeededForPowerup = 20;

	// Number of seconds to add to the powerup timer when picking up a powerup while its active
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PowerupPickupAddTime = 0.5f;

	// Sound to play when adding powerup time
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class USoundBase> PowerupTimeAddSound;

	// Currently playing powerup time add sound
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class UAudioComponent> CurrentPowerupTimeAddSound;

	// -------------------------------------------------------------------------------------
};
