// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Tile.h"
#include "FlabryaGameInstance.generated.h"
/**
 * 
 */

USTRUCT(BlueprintType)
struct FLeader
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Score;

};


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
		bool bIsLoading = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsSoundOn = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FLeader> Leaders;

	UFUNCTION(BlueprintCallable, Category = Initialization)
		void SaveGame();

	UFUNCTION(BlueprintCallable, Category = Initialization)
		void RemoveTopLeaders();

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

	UFUNCTION(BlueprintCallable, Category = Initialization)
		void AddLeader(FLeader Leader);

	UFUNCTION(BlueprintCallable, Category = Initialization)
		FString LeadersToString();

};
