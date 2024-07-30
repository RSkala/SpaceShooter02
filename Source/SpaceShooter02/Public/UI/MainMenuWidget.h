// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

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
class SPACESHOOTER02_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, meta = (DeprecatedFunction, DeprecationMessage = "Function has been deprecated, use NativeTick"))
	void OnTick(float DeltaTime);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/*UMG_API virtual void NativeOnInitialized();
	UMG_API virtual void NativePreConstruct();
	UMG_API virtual void NativeConstruct();
	UMG_API virtual void NativeDestruct();
	UMG_API virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);*/

	void UpdateMainMenuTitleTextColor(float DeltaTime);
	void SwitchToNextColorCyclingMode();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> MainMenuTitleText;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EColorCyclingMode ColorCyclingMode = EColorCyclingMode::RedToGreen;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ColorCyclingTimer = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SecondsPerCyclingMode = 1.0f; // How many seconds a color-cycling mode will last
};
