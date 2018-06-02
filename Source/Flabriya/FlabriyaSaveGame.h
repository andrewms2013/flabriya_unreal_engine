// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Tile.h"
#include "FlabriyaSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class FLABRIYA_API UFlabriyaSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

	UFlabriyaSaveGame();

	UPROPERTY(VisibleAnywhere, Category = Basic)
		FString LevelName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		TArray<int32> GameTiles;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		int32 Score;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		int32 SecondsLeft;

};
