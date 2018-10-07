// Fill out your copyright notice in the Description page of Project Settings.

#include "FlabriyaGameController.h"


AFlabriyaGameController::AFlabriyaGameController()
{
	PrimaryActorTick.bCanEverTick = true;
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	bEnableMouseOverEvents = true;
	bEnableTouchOverEvents = true;
}

