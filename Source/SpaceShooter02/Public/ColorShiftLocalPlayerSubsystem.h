// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "ColorShiftLocalPlayerSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FColorShiftDelegateSignature, FLinearColor, LinearColor);

UENUM(BlueprintType)
enum class EColorCyclingMode : uint8
{
	RedToGreen,
	GreenToBlue,
	BlueToRed,
	NumColorCyclingModes UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EColorCyclingMode, EColorCyclingMode::NumColorCyclingModes); // This is REQUIRED in order to iterate over it in C++ (i.e. TEnumRange)


UCLASS()
class SPACESHOOTER02_API UColorShiftLocalPlayerSubsystem : public ULocalPlayerSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	// ULocalPlayerSubsystem Begin
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection);
	virtual void Deinitialize();
	// ULocalPlayerSubsystem End

	// FTickableGameObject Begin
	virtual void Tick(float DeltaTime) override;
	virtual ETickableTickType GetTickableTickType() const override;
	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(UColorShiftLocalPlayerSubsystem, STATGROUP_Tickables);
	}
	virtual bool IsTickableWhenPaused() const { return false; }
	virtual bool IsTickableInEditor() const { return false; }
	// FTickableGameObject End

private:
	void UpdateColorShift(float DeltaTime);
	void SwitchToNextColorCyclingMode();
	static FString ColorShiftEnumToString(EColorCyclingMode ColorCyclingMode);

public:
	FColorShiftDelegateSignature OnColorShift;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EColorCyclingMode ColorCyclingMode = EColorCyclingMode::RedToGreen;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float ColorCyclingTimer = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.1", UIMin = "0.1", AllowPrivateAccess = true))
	float SecondsPerCyclingMode = 1.0f; // How many seconds a color-cycling mode will last

	// The last frame number that was ticked. Used to prevent ticking multiple times per frame.
	uint32 LastFrameNumberTicked = INDEX_NONE;
};
