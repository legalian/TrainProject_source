// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TrainProjectGameMode.h"
#include "TrainProjectPlayerController.h"
#include "TrainProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATrainProjectGameMode::ATrainProjectGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATrainProjectPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}