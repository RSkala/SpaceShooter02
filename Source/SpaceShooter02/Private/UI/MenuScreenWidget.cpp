// Copyright 2024 Richard Skala

#include "UI/MenuScreenWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

#include "ColorShiftLocalPlayerSubsystem.h"

DEFINE_LOG_CATEGORY_CLASS(UMenuScreenWidget, LogMenus)

void UMenuScreenWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Subscribe to the color shift delegate
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (UColorShiftLocalPlayerSubsystem* ColorShiftSubsystem = ULocalPlayer::GetSubsystem<UColorShiftLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			ColorShiftSubsystem->OnColorShift.AddUniqueDynamic(this, &ThisClass::OnColorShift);
		}
	}

	// Disable Hit Test on border images
	DisableHitTestForImage(Image_Screen_Border_L);
	DisableHitTestForImage(Image_Screen_Border_R);
	DisableHitTestForImage(Image_Screen_Border_T);
	DisableHitTestForImage(Image_Screen_Border_B);

	// Get notified when the viewport resizes
	ViewportResizeHandle = FViewport::ViewportResizedEvent.AddUObject(this, &ThisClass::OnViewportResized);

	// Get notified when the application gets and loses focus
	ApplicationFocusChangedHandle = FSlateApplication::Get().OnApplicationActivationStateChanged().AddUObject(this, &ThisClass::OnApplicationActivationStateChanged);
}

void UMenuScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UButton* KeyboardFocusLostButton = GetKeyboardFocusLostButton();
	if (KeyboardFocusLostButton != nullptr)
	{
		KeyboardFocusLostButton->SetKeyboardFocus();
	}
}

void UMenuScreenWidget::NativeDestruct()
{
	Super::NativeDestruct();

	// Unsubscribe from the viewport resize event
	if (ViewportResizeHandle.IsValid())
	{
		FViewport::ViewportResizedEvent.Remove(ViewportResizeHandle);
		ViewportResizeHandle.Reset();
	}

	// Unsubscribe from the application change focus event
	if (ApplicationFocusChangedHandle.IsValid())
	{
		FSlateApplication::Get().OnApplicationActivationStateChanged().Remove(ApplicationFocusChangedHandle);
		ApplicationFocusChangedHandle.Reset();
	}
}

void UMenuScreenWidget::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);
}

void UMenuScreenWidget::NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent)
{
	// Potential alternate fix for mouse losing focus when clicking outside any widget:
	// https://forums.unrealengine.com/t/my-buttons-lose-keyboard-focus-if-a-mouse-is-clicked/1150836/10
	// https://forums.unrealengine.com/t/common-ui-and-keyboard-only-no-mouse/1234750/4

	//TWeakPtr<SWidget> PreviousWidget = PreviousFocusPath.IsValid() ? PreviousFocusPath.GetLastWidget() : nullptr;
	//FName PreviousWidgetTypeName = PreviousWidget.IsValid() ? PreviousWidget.Pin().Get()->GetType() : NAME_None;

	FName NewWidgetTypeName = NAME_None;
	if (NewWidgetPath.IsValid()) // Need to check IsValid, otherwise GetLastWidget() could error out (happens on viewport resize, e.g. via F11 in PIE)
	{
		TSharedRef<SWidget> NewWidget = NewWidgetPath.GetLastWidget();
		NewWidgetTypeName = NewWidget.Get().GetType();
	}

	//UE_LOG(LogTemp, Warning, TEXT("----------------------------------"));
	//UE_LOG(LogTemp, Warning, TEXT("PreviousWidgetTypeName: %s"), *PreviousWidgetTypeName.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("NewWidgetTypeName:      %s"), *NewWidgetTypeName.ToString());

	if (InFocusEvent.GetCause() == EFocusCause::Mouse && (NewWidgetTypeName == FName("SPIEViewport") || NewWidgetTypeName == FName("SViewport")))
	{
		UButton* KeyboardFocusLostButton = GetKeyboardFocusLostButton();
		if (KeyboardFocusLostButton != nullptr)
		{
			UE_LOG(LogMenus, Warning, TEXT("User clicking on a viewport. Forcing keyboard focus to %s"), *KeyboardFocusLostButton->GetName());
			KeyboardFocusLostButton->SetKeyboardFocus();
			return;
		}
	}

	Super::NativeOnFocusChanging(PreviousFocusPath, NewWidgetPath, InFocusEvent);
}

FNavigationReply UMenuScreenWidget::NativeOnNavigation(const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply)
{
	//return Super::NativeOnNavigation(MyGeometry, InNavigationEvent, InDefaultReply);
	return FNavigationReply::Stop();
}

FNavigationReply UMenuScreenWidget::NativeOnNavigation(const FGeometry& InGeometry, const FNavigationEvent& NavigationEvent)
{
	return FNavigationReply::Stop();
}

void UMenuScreenWidget::OnColorShift(FLinearColor LinearColor)
{
	SetColorForImage(Image_Screen_Border_L, LinearColor);
	SetColorForImage(Image_Screen_Border_R, LinearColor);
	SetColorForImage(Image_Screen_Border_T, LinearColor);
	SetColorForImage(Image_Screen_Border_B, LinearColor);
}

void UMenuScreenWidget::SetColorForImage(UImage* Image, FLinearColor LinearColor)
{
	if (Image != nullptr)
	{
		Image->SetColorAndOpacity(LinearColor);
	}
}

void UMenuScreenWidget::DisableHitTestForImage(UImage* Image)
{
	if (Image != nullptr)
	{
		Image->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UMenuScreenWidget::SetColorShiftForButton(UButton* Button, FLinearColor ShiftColor)
{
	if (Button != nullptr)
	{
		FLinearColor LinearColor = Button->IsHovered() || Button->HasKeyboardFocus() ? ShiftColor : FLinearColor::White;
		//Button->SetColorAndOpacity(LinearColor); // This sets the text color
		Button->SetBackgroundColor(LinearColor);
	}
}

void UMenuScreenWidget::OnViewportResized(FViewport* InViewport, uint32 InParams)
{
	// We want to force a keyboard focus on viewport resize. There is a known issue 
	// where keyboard focus will be lost in various circumstances, viewport resize being
	// one of several, and that is what we have to handle here.

	// After a viewport resize, there are no focused widgets so forced the focus now
	UWidget* FocusedWidget = GetFocusedWidget();
	if (FocusedWidget == nullptr)
	{
		UButton* KeyboardFocusLostButton = GetKeyboardFocusLostButton();
		if (KeyboardFocusLostButton != nullptr)
		{
			UE_LOG(LogMenus, Log, TEXT("No focused widget on resize. Forcing to %s"), *KeyboardFocusLostButton->GetName());
			KeyboardFocusLostButton->SetKeyboardFocus();
		}
	}
}

void UMenuScreenWidget::OnApplicationActivationStateChanged(bool bIsActive)
{
	if (!bIsActive)
	{
		return;
	}

	UWidget* FocusedWidget = GetFocusedWidget();
	if (FocusedWidget == nullptr)
	{
		UButton* KeyboardFocusLostButton = GetKeyboardFocusLostButton();
		if (KeyboardFocusLostButton != nullptr)
		{
			UE_LOG(LogMenus, Log, TEXT("No focused widget after application window focus change. Forcing to %s"), *KeyboardFocusLostButton->GetName());
			KeyboardFocusLostButton->SetKeyboardFocus();
		}
	}
}

UWidget* UMenuScreenWidget::GetFocusedWidget() const
{
	// Get the currently focused widget
	// https://georgy.dev/posts/get-focused-widget/

	UWidget* FocusedWidget = nullptr;
	TSharedPtr<SWidget> FocusedSlateWidget = FSlateApplication::Get().GetUserFocusedWidget(0);
	if (!FocusedSlateWidget.IsValid())
	{
		for (TObjectIterator<UWidget> Itr; Itr; ++Itr)
		{
			UWidget* CandidateUMGWidget = *Itr;
			if (CandidateUMGWidget != nullptr && CandidateUMGWidget->GetCachedWidget() == FocusedSlateWidget)
			{
				FocusedWidget = CandidateUMGWidget;
				break;
			}
		}
	}
	return FocusedWidget;
}
