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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ScoreToWin;
	
	int Score = 0;

	static void AddOnePoint(UWorld* World);

	void PrintScoreToRenderComponent();

	void CheckIfWon();
};
