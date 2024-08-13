// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShipSelectionWidget.generated.h"

UCLASS()
class SPACESHOOTER02_API UShipSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitShipSelectionWidget( // TODO: Use a data asset and pass the data asset
		FString ShipNameString,
		FString ShipAbilityString,
		FLinearColor ShipAbilityColor,
		int32 InShipSpriteIndex
	);

	class UButton* GetLaunchButton() const { return LaunchButton; }

protected:
	virtual void NativeOnInitialized() override;
	virtual FNavigationReply NativeOnNavigation(const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply) override;
	virtual FNavigationReply NativeOnNavigation(const FGeometry& InGeometry, const FNavigationEvent& InNavigationEvent) override;

private:
	UFUNCTION()
	void OnLaunchButtonClicked();

	void ShipSelected();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UTextBlock> ShipNameTextBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UTextBlock> ShipAbilityTextBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UImage> ShipImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UButton> LaunchButton;

	// Index into the Ship Sprite list in the game instance
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 ShipSpriteIndex = -1;
};
