// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SpaceShooterGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameStartedDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameEndedDelegateSignature, int32, FinalScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerScoreChangedDelegateSignature, int32, PlayerScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerMultiplierChangedDelegateSignature, int32, ScoreMultiplier);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHighScoreChangedDelegateSignature, int32, HighScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAddSatelliteWeaponDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPickupItemPercentChanged, float, Percent);


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
	void EndGame(int32 FinalScore);

	int32 GetPlayerScore() const { return PlayerScore; }

	void AddCurrentScoreMultiplier(int32 AmountToAdd);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPlayerShipSpawned(class APlayerShipPawn* const InPlayerShipPawn);
	UFUNCTION()
	void OnPlayerShipDestroyed();
	UFUNCTION()
	void OnEnemyDeath(FVector EnemyDeathPosition, class UNiagaraSystem* EnemyDeathEffect, class USoundBase* EnemyDeathSound);
	UFUNCTION()
	void OnScoreMultiplierPickedUp(int32 ScoreMultiplierValue);

	UFUNCTION()
	void OnSatelliteWeaponPickedUp();

	// --- Menu Delegate Handlers ---

	UFUNCTION()
	void OnMainMenuPlayClicked(); // When the player clicks "Play" from the Main Menu
	UFUNCTION()
	void OnPlayerShipSelected(class UPaperSprite* SelectedShipSprite); // When the player selects their ship (from the Ship Select screen)
	UFUNCTION()
	void OnGameOverSelectShipSelected(); // When the user selects "Select Ship" from the Game Over screen
	UFUNCTION()
	void OnGameOverPlayAgainSelected(); // When the user selects "Play Again" from the Game Over screen

	void SpawnScoreMultiplierPickup(FVector SpawnPosition);

	void OnGameOverTimerTimeout(int32 FinalScore);

public:
	static FGameStartedDelegateSignature OnGameStarted; // Delegate called when the player starts a game (either from main menu or game over)
	static FGameEndedDelegateSignature OnGameEnded; // Delegate called when the player is defeated (game over)
	static FPlayerScoreChangedDelegateSignature OnPlayerScoreChanged; // Delegate called when the player's current score is updated
	static FPlayerMultiplierChangedDelegateSignature OnPlayerMultiplierChanged; // Delegate called when the player's current multiplier is updated
	static FHighScoreChangedDelegateSignature OnPlayerHighScoreChanged; // Delegate called when the player beats the current high score
	static FAddSatelliteWeaponDelegateSignature OnAddSatelliteWeapon; // Delegate called when player has picked up a satellite weapon
	static FPickupItemPercentChanged OnPickupItemPercentChanged; // Called when num pickups changed. Passes percent of total required for powerup.

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

	// --- Pickups ---

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class APickupItemBase> ScoreMultiplierPickupItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float ScoreMultiplierDropChance = 0.5f;

	//// Total multipliers collected during game. Currently unused. May use for savegames/leaderboards, etc.
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	//int32 TotalMultipliersCollected = 0;

	//// Total multipliers collected for powerup
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	//int32 NumMultipliersCollectedForPowerup = 0;

	//// Number of multiplier pickups needed to be collected to "activate" powerup
	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//int32 NumMultipliersNeededForPowerup = 20;

	// Amount of time to delay after a game over before sending notification (which delays game over screen and such)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DelayAfterGameOver = 1.5f;

	// Sound to play when collecting a pickup item
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class USoundBase> ScoreMultiplierPickupSound;

	// Currently playing pickup item sound
	UPROPERTY()
	TObjectPtr<class UAudioComponent> CurrentMultiplierPickupSound;
};
