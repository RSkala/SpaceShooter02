// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SpaceShooterGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameStartedDelegateSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SevenParams(
	FGameEndedDelegateSignature,
	int32, FinalScore,
	int32, SelectedShipSpriteIndex,
	int32, NumEnemiesDefeated,
	int32, NumScoreMultipliersCollected,
	int32, NumEnemiesDefeatedWithBoost,
	int32, NumProjectilesFired,
	int32, CurrentScoreMultiplier);


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerScoreChangedDelegateSignature, int32, PlayerScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerMultiplierChangedDelegateSignature, int32, ScoreMultiplier);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHighScoreChangedDelegateSignature, int32, HighScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAddSatelliteWeaponDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPickupItemPercentChanged, float, Percent);

// Delegates for pausing / unpausing the game
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRequestPauseGameDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRequestUnpauseGameDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRequestSelfDestructDelegateSignature);

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

	void StartGame();
	void EndGame(int32 FinalScore);

	int32 GetPlayerScore() const { return PlayerScore; }

	void AddCurrentScoreMultiplier(int32 AmountToAdd);

	// -----------------------------------------------------------------------------
	// --- Difficulty Scaling ---

	float GetTimeBetweenSpawns() const { return CurrentTimeBetweenSpawns; }

	void FireProjectile(FVector ProjectilePosition, FRotator ProjectileRotation, APawn* InInstigator);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPlayerShipSpawned(class APlayerShipPawn* const InPlayerShipPawn);
	UFUNCTION()
	void OnPlayerShipDestroyed();
	UFUNCTION()
	void OnEnemyDeath(FVector EnemyDeathPosition, class UNiagaraSystem* EnemyDeathEffect, bool bKilledFromBoost);
	UFUNCTION()
	void OnScoreMultiplierPickedUp(int32 ScoreMultiplierValue);

	UFUNCTION()
	void OnSatelliteWeaponPickedUp();

	// --- Menu Delegate Handlers ---

	UFUNCTION()
	void OnMainMenuPlayClicked(); // When the player clicks "Play" from the Main Menu
	UFUNCTION()
	void OnPlayerShipSelected(int32 InSelectedShipSpriteIndex); // When the player selects their ship (from the Ship Select screen)
	UFUNCTION()
	void OnGameOverSelectShipSelected(); // When the user selects "Select Ship" from the Game Over screen
	UFUNCTION()
	void OnGameOverPlayAgainSelected(); // When the user selects "Play Again" from the Game Over screen

	void SpawnScoreMultiplierPickup(FVector SpawnPosition);

	void OnGameOverTimerTimeout(int32 FinalScore);

	UFUNCTION()
	void HandleRequestPauseGame();

	UFUNCTION()
	void HandleRequestUnpauseGame();

	UFUNCTION()
	void HandleRequestSelfDestruct();

public:
	static FGameStartedDelegateSignature OnGameStarted; // Delegate called when the player starts a game (either from main menu or game over)
	static FGameEndedDelegateSignature OnGameEnded; // Delegate called when the player is defeated (game over)
	static FPlayerScoreChangedDelegateSignature OnPlayerScoreChanged; // Delegate called when the player's current score is updated
	static FPlayerMultiplierChangedDelegateSignature OnPlayerMultiplierChanged; // Delegate called when the player's current multiplier is updated
	static FHighScoreChangedDelegateSignature OnPlayerHighScoreChanged; // Delegate called when the player beats the current high score
	static FAddSatelliteWeaponDelegateSignature OnAddSatelliteWeapon; // Delegate called when player has picked up a satellite weapon
	static FPickupItemPercentChanged OnPickupItemPercentChanged; // Called when num pickups changed. Passes percent of total required for powerup.

	static FRequestPauseGameDelegateSignature OnRequestPauseGame;
	static FRequestUnpauseGameDelegateSignature OnRequestUnpauseGame;
	static FRequestSelfDestructDelegateSignature OnRequestSelfDestruct;

protected:
	// Player's score during the current game
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 PlayerScore = 0;

	// Class for creating the enemy spawner
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class AEnemySpawner> EnemySpawnerClass;

	// Actual enemy spawner object
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TObjectPtr<class AEnemySpawner> EnemySpawner;

	// Menu state -- currently unused
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EShooterMenuGameState ShooterMenuGameState;

	// Class for creating menu controller instance
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class USpaceShooterMenuController> MenuControllerClass;

	// MenuController instance
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TObjectPtr<class USpaceShooterMenuController> MenuController;

	// Reference to player's ship
	UPROPERTY()
	TSoftObjectPtr<class APlayerShipPawn> PlayerShipPawn;

	// Player's score multiplier during the current game
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 CurrentScoreMultiplier = 1;

	// Enemy score value -- TODO: Move to enemy class or data object
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 EnemyScoreValue = 1; // first implementation: all enemies have a score value of 1

	// The player's high score at the start of a game
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 PlayerHighScore = 0;

	// --- Pickups ---

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

	// ----------------------------------------------------------
	// --------------------------
	// --- Difficulty Scaling ---
	// --------------------------

	// Number of enemies spawned in the current game 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 TotalNumEnemiesSpawnedThisGame = 0;

	// Number of enemies killed by the player in the current game
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 TotalNumEnemiesKilledThisGame = 0;

	// Number of score multipliers collected in the current game
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 TotalNumScoreMultipliersCollectedThisGame = 0;

	// Number of enemies killed using the boost ability
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 TotalNumEnemiesKilledWithBoostThisGame = 0;

	// Number of projectiles fired during the current game
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 TotalProjectilesFiredThisGame = 0;

	// Every X enemy spawned or killed, increase difficulty
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 DifficultySpikeInterval = 10;

	// Keeps track of currently difficulty spike level
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 CurrentDifficultyLevel = 1;

	// Amount of time that needs to elapse between spawned enemies (seconds)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentTimeBetweenSpawns = 1.0f;

	// Time between spawns should be decreased no lower than this
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeBetweenSpawnsAbsoluteMinimum = 0.0625;

	// Amount of time to decrease between spawns at each difficulty spike increase
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeBetweenSpawnDecreaseAmount = 0.05f;

	// ----------------------------------------------------------

	// --- Projectile Controller ---

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UProjectileController> ProjectileControllerClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TObjectPtr<class UProjectileController> ProjectileController;

	// --- Pickup Item Controller

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UPickupItemController> PickupItemControllerClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TObjectPtr<class UPickupItemController> PickupItemController;

	// --- Explosion Sprite Controller ---

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UExplosionSpriteController> ExplosionSpriteControllerClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TObjectPtr<class UExplosionSpriteController> ExplosionSpriteController;

	// --- Spawn Anim Controller ---

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class USpawnAnimController> SpawnAnimControllerClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TObjectPtr<class USpawnAnimController> SpawnAnimController;

	// --- Enemy Controller ---

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UEnemyPoolController> EnemyPoolControllerClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TObjectPtr<class UEnemyPoolController> EnemyPoolController;

	// ----------------------------------------------------------
	// The player's currently selected ship sprite index
	int32 SelectedShipSpriteIndex = -1;
};
