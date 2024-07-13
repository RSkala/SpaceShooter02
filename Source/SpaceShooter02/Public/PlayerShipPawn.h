// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerShipPawn.generated.h"

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void UpdateMovement(float DeltaTime);
	void UpdateExhaust();

protected:
	void KeyboardMoveTriggered(const struct FInputActionValue& InputActionValue);
	void KeyboardMoveCompleted(const struct FInputActionValue& InputActionValue);

	void GamepadMoveTriggered(const struct FInputActionValue& InputActionValue);
	void GamepadMoveCompleted(const struct FInputActionValue& InputActionValue);

	void GamepadAimTriggered(const struct FInputActionValue& InputActionValue);
	void GamepadAimCompleted(const struct FInputActionValue& InputActionValue);

	void Fire(const struct FInputActionValue& InputActionValue);

protected:
	// --- Components ---

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerShipPawn|Components")
	TObjectPtr<class USphereComponent> SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerShipPawn|Components")
	TObjectPtr<class UPaperSpriteComponent> PaperSpriteComp;

	// --- Camera ---
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerShipPawn|Components|Camera")
	TObjectPtr<class USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerShipPawn|Components|Camera")
	TObjectPtr<class UCameraComponent> CameraComp;

	// --- Scene Components (fire point, exhaust, etc) ---

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerShipPawn|Components|Scene")
	TObjectPtr<class USceneComponent> FirePointComp;

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

	// --- Movement and Aiming ---
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerShipPawn|Movement & Aiming")
	FVector2D MovementDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerShipPawn|Movement & Aiming")
	FVector2D AimingDirection;

	// --- Movement ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerShipPawn|Movement & Aiming", meta = (ClampMin = "1"))
	float MoveSpeed;
};