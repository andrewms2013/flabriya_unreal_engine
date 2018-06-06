// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "FlabriyaGameMode.h"
#include "FlabriyaGameController.h"

AFlabriyaGameMode::AFlabriyaGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DefaultPawnClass = APawn::StaticClass();
	PlayerControllerClass = AFlabriyaGameController::StaticClass();
}