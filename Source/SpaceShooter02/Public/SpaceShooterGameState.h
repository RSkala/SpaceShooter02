// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SpaceShooterGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameStartedDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameEndedDelegateSignature);

UENUM(BlueprintType)
enum class EShooterMenuGameState : uint8
{
	MainMenu,
	ShipSelect,
	Gameplay,
	GameOver,
	NumMenuGameStates UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EShooterMenuGameState, EShooterMenuGameState::NumMenuGameStates);

UCLASS()
class SPACESHOOTER02_API ASpaceShooterGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ASpaceShooterGameState();
	int32 GetNumEnemiesDefeated() const { return NumEnemiesDefeated; }
	void IncrementNumEnemiesDefeated() { NumEnemiesDefeated++; }

	void StartGame();
	void EndGame();

protected:
	virtual void BeginPlay() override;

public:
	// Delegate called when the player starts a game (either from main menu or game over)
	static FGameStartedDelegateSignature OnGameStarted;

	// Delegate called when the player is defeated (game over)
	static FGameEndedDelegateSignature OnGameEnded;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 NumEnemiesDefeated = 0;

	// Class for creating the enemy spawner
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class AEnemySpawner> EnemySpawnerClass;

	// Actual enemy spawner object
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TObjectPtr<class AEnemySpawner> EnemySpawner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EShooterMenuGameState ShooterMenuGameState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class USpaceShooterMenuController> MenuControllerClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USpaceShooterMenuController> MenuController;

	// Reference to player's ship
	TSoftObjectPtr<class APlayerShipPawn> PlayerShipPawn;
};
