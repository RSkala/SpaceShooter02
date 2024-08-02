// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SpaceShooterGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameStartedDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameEndedDelegateSignature); // TODO int32, FinalScore
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerScoreChangedDelegateSignature, int32, PlayerScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerMultiplierChangedDelegateSignature, int32, ScoreMultiplier);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHighScoreChangedDelegateSignature, int32, HighScore);


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

	int32 GetPlayerScore() const { return PlayerScore; }

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPlayerShipSpawned(class APlayerShipPawn* const InPlayerShipPawn);

	UFUNCTION()
	void OnPlayerShipDestroyed();

	UFUNCTION()
	void OnEnemyDeath(FVector EnemyDeathPosition, class UNiagaraSystem* EnemyDeathEffect);

public:
	static FGameStartedDelegateSignature OnGameStarted; // Delegate called when the player starts a game (either from main menu or game over)
	static FGameEndedDelegateSignature OnGameEnded; // Delegate called when the player is defeated (game over)
	static FPlayerScoreChangedDelegateSignature OnPlayerScoreChanged; // Delegate called when the player's current score is updated
	static FPlayerMultiplierChangedDelegateSignature OnPlayerMultiplierChanged; // Delegate called when the player's current multiplier is updated
	static FHighScoreChangedDelegateSignature OnPlayerHighScoreChanged; // Delegate called when the player beats the current high score

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 NumEnemiesDefeated = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 PlayerScore = 0;

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
	UPROPERTY()
	TSoftObjectPtr<class APlayerShipPawn> PlayerShipPawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 CurrentScoreMultiplier = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 EnemyScoreValue = 1; // first implementation: all enemies have a score value of 1

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 PlayerHighScore = 0;
};
