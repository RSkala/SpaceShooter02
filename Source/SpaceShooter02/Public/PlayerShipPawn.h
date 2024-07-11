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

protected:
	void MoveTriggered(const struct FInputActionValue& InputActionValue);
	void MoveCompleted(const struct FInputActionValue& InputActionValue);
	void Fire(const struct FInputActionValue& InputActionValue);

protected:
	// --- Components ---

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USphereComponent> SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UPaperSpriteComponent> PaperSpriteComp;

	// --- Camera ---
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCameraComponent> CameraComp;

	// --- Input ---
	// ==== RKS TODO: First, get general input working, then move all input into a PlayerController subclass

	// Mapping Context for Input
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerShipPawn|Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	// Player Ship Movement, i.e. Left Thumbstick or WASD
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerShipPawn|Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputAction> InputActionMove;

	// Player Ship Aim, i.e. Right Thumbstick or Mouse
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerShipPawn|Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputAction> InputActionAim;

	// Player Ship Shoot, i.e. Gamepad face button or Left Mouse
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerShipPawn|Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputAction> InputActionFire;

	// Player Ship Dash, i.e. Gamepad right bumpter or SpaceBar
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerShipPawn|Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputAction> InputActionDash;
};
