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
	// --- Components ---

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class USphereComponent> SphereComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UPaperSpriteComponent> PaperSpriteComp;

	// --- Camera ---
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class USpringArmComponent> SpringArmComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UCameraComponent> CameraComp;
};
