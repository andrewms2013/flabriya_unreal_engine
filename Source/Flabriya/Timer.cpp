// Fill out your copyright notice in the Description page of Project Settings.

#include "Timer.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Engine.h"
#include "Grid.h"
#include "ScoreCounter.h"
#include "EngineUtils.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "OpenSettingButton.h"
#include "FlabryaGameInstance.h"

// Sets default values
ATimer::ATimer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ATimer::InitTimer() {

	PrintTimeToRenderComponent();
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
		FStringClassReference MyWidgetClassRef;
		if (GetWorld()->GetName() == "FlabriyaLevel")
		{
			MyWidgetClassRef.SetPath(TEXT("/Game/InGameWidgets/LevelLostPink.LevelLostPink_C"));
		}
		else if (GetWorld()->GetName() == "SirenLevel")
		{
			MyWidgetClassRef.SetPath(TEXT("/Game/InGameWidgets/LevelLostGreen.LevelLostGreen_C"));
		}
		else if (GetWorld()->GetName() == "EndlessLevel")
		{
			FLeader Leader;
			UFlabryaGameInstance * Instance = (UFlabryaGameInstance *)GetGameInstance();
			Leader.Name = Instance->PlayerName;
			Leader.Score = AScoreCounter::GetScore(GetWorld());
			Instance->AddLeader(Leader);
			MyWidgetClassRef.SetPath(TEXT("/Game/InGameWidgets/LevelWonEndless.LevelWonEndless_C"));
		}
		else
		{
			MyWidgetClassRef.SetPath(TEXT("/Game/InGameWidgets/LevelLostBrown.LevelLostBrown_C"));
		}
		APlayerController * MyPlayer = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (UClass* MyWidgetClass = MyWidgetClassRef.TryLoadClass<UUserWidget>())
		{
			UUserWidget* MyWidget = CreateWidget<UUserWidget>(MyPlayer, MyWidgetClass);
			MyWidget->AddToViewport();
		}
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


int32 ATimer::GetTimerLeftSeconds(UWorld* World) 
{
	for (TObjectIterator<ATimer> Itr; Itr; ++Itr)
	{
		if (Itr->GetWorld() != World)
		{
			continue;
		}
		return Itr->TimeLeftSeconds;
	}
	return 0;
}

