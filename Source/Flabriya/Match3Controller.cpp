// Fill out your copyright notice in the Description page of Project Settings.

#include "Match3Controller.h"

AMatch3Controller::AMatch3Controller(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	bShowMouseCursor = true;

	bEnableClickEvents = true;
	
}


