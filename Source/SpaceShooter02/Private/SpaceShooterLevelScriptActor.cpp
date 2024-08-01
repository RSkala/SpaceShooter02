// Copyright 2024 Richard Skala

#include "SpaceShooterLevelScriptActor.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

//#include "SpaceShooterGameState.h"
#include "UI/MainMenuScreen.h"

DEFINE_LOG_CATEGORY_STATIC(LogSpaceShooterLevelScript, Log, All)

void ASpaceShooterLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();
	StartMainMenu();
}

void ASpaceShooterLevelScriptActor::StartMainMenu()
{
	if (bSkipMainMenuLoad)
	{
		UE_LOG(LogSpaceShooterLevelScript, Warning, TEXT("Skipping Main Menu load"));

		// This is loaded before the GameState, so this doesn't work
		/*if (UWorld* World = GetWorld())
		{
			if (ASpaceShooterGameState* SpaceShooterGameState = World->GetGameState<ASpaceShooterGameState>())
			{
				SpaceShooterGameState->StartGame();
			}
		}*/
		return;
	}

	if (ensure(MainMenuScreenClass != nullptr))
	{
		UWorld* World = GetWorld();
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);

		UUserWidget* NewWidget = UWidgetBlueprintLibrary::Create(World, MainMenuScreenClass, PlayerController);
		MainMenuScreen = Cast<UMainMenuScreen>(NewWidget);

		//FInputModeGameAndUI InputMode;
		//InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		//InputMode.SetHideCursorDuringCapture(false);
		//PlayerController->SetInputMode(InputMode);
		//PlayerController->SetShowMouseCursor(true);
		MainMenuScreen->AddToViewport(0);
	}
}
