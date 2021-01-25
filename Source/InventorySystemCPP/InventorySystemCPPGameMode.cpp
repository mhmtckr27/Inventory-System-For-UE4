// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventorySystemCPPGameMode.h"
#include "InventorySystemCPPPlayerController.h"
#include "InventorySystemCPPCharacter.h"
#include "UObject/ConstructorHelpers.h"

AInventorySystemCPPGameMode::AInventorySystemCPPGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AInventorySystemCPPPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}