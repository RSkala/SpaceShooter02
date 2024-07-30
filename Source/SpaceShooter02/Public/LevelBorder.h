// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelBorder.generated.h"

// TODO: Move this into a LevelBorderController
UENUM(BlueprintType)
enum class EColorCyclingMode : uint8
{
	RedToGreen,
	GreenToBlue,
	BlueToRed,
	NumColorCyclingModes UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EColorCyclingMode, EColorCyclingMode::NumColorCyclingModes); // This is REQUIRED in order to iterate over it in C++ (i.e. TEnumRange)

// A level border defines the border of the playable area. A player cannot cross a LevelBorder.
UCLASS()
class SPACESHOOTER02_API ALevelBorder : public AActor
{
	GENERATED_BODY()
	
public:
	ALevelBorder();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	void UpdateLevelBorderSpriteColor(float DeltaTime);
	void SwitchToNextColorCyclingMode();

protected:

	// --- Components ---

	// Box collision for border
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UBoxComponent> BoxComp;

	// Visible sprite
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UPaperSpriteComponent> PaperSpriteComp;

	// --- Color Shift ---

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EColorCyclingMode ColorCyclingMode = EColorCyclingMode::RedToGreen;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ColorCyclingTimer = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.1", UIMin = "0.1"))
	float SecondsPerCyclingMode = 1.0f; // How many seconds a color-cycling mode will last
};
