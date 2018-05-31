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
		int TimeLeftSeconds;

	FTimerHandle GameTimer;

	void AdvanceTimer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	void PrintTimeToRenderComponent();
};
