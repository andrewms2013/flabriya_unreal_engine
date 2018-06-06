// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "FlabryaGameInstance.h"
#include "FlabriyaSaveLeaderboard.generated.h"

/**
 * 
 */
UCLASS()
class FLABRIYA_API UFlabriyaSaveLeaderboard : public USaveGame
{
	GENERATED_BODY()

public:

	UFlabriyaSaveLeaderboard();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FLeader> Leaders;
	
	
};
