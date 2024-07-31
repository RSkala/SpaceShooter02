// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

// TODO: Move to global so this doesn't have to be duplicated
UENUM(BlueprintType)
enum class EMainMenuColorCyclingMode : uint8
{
	RedToGreen,
	GreenToBlue,
	BlueToRed,
	NumColorCyclingModes UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EMainMenuColorCyclingMode, EMainMenuColorCyclingMode::NumColorCyclingModes); // This is REQUIRED in order to iterate over it in C++ (i.e. TEnumRange)

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

	void UpdateMainMenuTitleTextColor(float DeltaTime);
	void SwitchToNextColorCyclingMode();

	void SetColorForTextBlock(class UTextBlock* TextBlock, FLinearColor LinearColor);
	void SetColorForImage(class UImage* Image, FLinearColor LinearColor);

protected:

	// --- Main Menu Elements ----

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> MainMenuTitleText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UImage> Image_Border_Left;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UImage> Image_Border_Right;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UImage> Image_Border_Top;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UImage> Image_Border_Bottom;


	// --- Color Cycling ---

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EMainMenuColorCyclingMode ColorCyclingMode = EMainMenuColorCyclingMode::RedToGreen;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ColorCyclingTimer = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SecondsPerCyclingMode = 1.0f; // How many seconds a color-cycling mode will last
};
