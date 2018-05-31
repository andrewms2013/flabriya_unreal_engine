// Fill out your copyright notice in the Description page of Project Settings.

#include "Timer.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Engine.h"
#include "Grid.h"
#include "EngineUtils.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "OpenSettingButton.h"

// Sets default values
ATimer::ATimer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ATimer::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(GameTimer, this, &ATimer::AdvanceTimer, 1.0f, true);
}

void ATimer::AdvanceTimer() 
{
	TimeLeftSeconds -= 1;
	PrintTimeToRenderComponent();
	if (TimeLeftSeconds == 0)
	{
		GetWorldTimerManager().ClearTimer(GameTimer);
		AGrid::SetGridRespondable(false, GetWorld());
		AOpenSettingButton::SetButtonRespondable(false, GetWorld());
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
}

void ATimer::PrintTimeToRenderComponent() 
{
	FString TimeString("Timer: ");
	TimeString += FString::FromInt(TimeLeftSeconds/60);
	TimeString += FString(":");
	if (TimeLeftSeconds % 60 >= 10)
	{
		TimeString += FString::FromInt(TimeLeftSeconds % 60);
	}
	else
	{
		TimeString += FString("0");
		TimeString += FString::FromInt(TimeLeftSeconds % 60);
	}
	TArray<UTextRenderComponent*> StaticComps;
	GetComponents<UTextRenderComponent>(StaticComps);
	if (StaticComps.IsValidIndex(0))
	{
		UTextRenderComponent* TextComponent = StaticComps[0];
		TextComponent->SetText(TimeString);
	}
}

