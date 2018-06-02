// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Timer.generated.h"

UCLASS()
class FLABRIYA_API ATimer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsEndless;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 TimeLeftSeconds;

	UFUNCTION(BlueprintCallable, Category = Initialization)
		void InitTimer();

	FTimerHandle GameTimer;

	void AdvanceTimer();

	static int32 GetTimerLeftSeconds(UWorld* World);
	
private:

	void PrintTimeToRenderComponent();

};
