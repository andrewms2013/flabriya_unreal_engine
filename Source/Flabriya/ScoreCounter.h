// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScoreCounter.generated.h"

UCLASS()
class FLABRIYA_API AScoreCounter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScoreCounter();

	UFUNCTION(BlueprintCallable, Category = Initialization)
		void InitScoreCounter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ScoreToWin;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsEndless;

	int32 Score = 0;

	void CheckIfWon();

	static void AddOnePoint(UWorld* World);

	static int32 GetScore(UWorld* World);

private:

	void PrintScoreToRenderComponent();

};
