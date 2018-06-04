// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Tile.h"
#include "FlabryaGameInstance.generated.h"
/**
 * 
 */
UCLASS()
class FLABRIYA_API UFlabryaGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UFlabryaGameInstance();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 LevelsWon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsNameWritten = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsLoads = false;

	UFUNCTION(BlueprintCallable, Category = Initialization)
		void SaveGame();

	UFUNCTION(BlueprintCallable, Category = Initialization)
		void SetForLoadAndOpenLevel();

	UFUNCTION(BlueprintCallable, Category = Initialization)
		void LoadGame();

	UFUNCTION(BlueprintCallable, Category = Initialization)
		void EnterNameAndOpenMenu();

	UFUNCTION(BlueprintCallable, Category = Initialization)
		void SaveProgress(int32 LvlWon);

	UFUNCTION(BlueprintCallable, Category = Initialization)
		void LoadProgress();
};
