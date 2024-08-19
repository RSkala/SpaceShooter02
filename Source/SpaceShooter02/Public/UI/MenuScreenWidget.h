// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuScreenWidget.generated.h"

UCLASS(Abstract)
class SPACESHOOTER02_API UMenuScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_LOG_CATEGORY_CLASS(LogMenus, Log, All)

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual bool NativeSupportsCustomNavigation() const override { return true; }
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent) override;
	virtual FNavigationReply NativeOnNavigation(const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply) override;
	virtual FNavigationReply NativeOnNavigation(const FGeometry& InGeometry, const FNavigationEvent& NavigationEvent) override;
	
	UFUNCTION()
	virtual void OnColorShift(FLinearColor LinearColor);

	void SetColorForImage(class UImage* Image, FLinearColor LinearColor);
	void DisableHitTestForImage(class UImage* Image);

	void SetColorShiftForButton(class UButton* Button, FLinearColor ShiftColor);

	virtual class UButton* GetKeyboardFocusLostButton() const
	{
		ensureAlwaysMsgf(
			false,
			TEXT("%s must be implemented in UMenuScreenWidget subclasses (%s)"),
				*GET_FUNCTION_NAME_CHECKED(UMenuScreenWidget, GetKeyboardFocusLostButton).ToString(),
				*this->GetClass()->GetName());
		return nullptr;
	}

	void OnViewportResized(class FViewport* InViewport, uint32 InParams);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UImage> Image_Screen_Border_L;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UImage> Image_Screen_Border_R;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UImage> Image_Screen_Border_T;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UImage> Image_Screen_Border_B;

	FDelegateHandle ViewportResizeHandle;
};
