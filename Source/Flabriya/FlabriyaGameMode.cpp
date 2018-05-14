// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "FlabriyaGameMode.h"
#include "Match3Controller.h"

AFlabriyaGameMode::AFlabriyaGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DefaultPawnClass = APawn::StaticClass();
	PlayerControllerClass = AMatch3Controller::StaticClass();
}
