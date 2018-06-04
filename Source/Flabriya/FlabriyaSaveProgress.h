// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "FlabriyaSaveProgress.generated.h"

/**
 * 
 */
UCLASS()
class FLABRIYA_API UFlabriyaSaveProgress : public USaveGame
{
	GENERATED_BODY()
	
public:

	UFlabriyaSaveProgress();

	UPROPERTY(VisibleAnywhere, Category = Basic)
		int32 LevelsWon;
	
	
};
