// Fill out your copyright notice in the Description page of Project Settings.

#include "ScoreCounter.h"
#include "EngineUtils.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "OpenSettingButton.h"
#include "Grid.h"
#include "Engine.h"

// Sets default values
AScoreCounter::AScoreCounter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AScoreCounter::InitScoreCounter()
{
	PrintScoreToRenderComponent();
}

void AScoreCounter::AddOnePoint(UWorld* World)
{
	for (TObjectIterator<AScoreCounter> Itr; Itr; ++Itr)
	{
		if (Itr->GetWorld() != World)
		{
			continue;
		}
		Itr->Score++;
		Itr->PrintScoreToRenderComponent();
		if (!(Itr->bIsEndless))
		{
			Itr->CheckIfWon();
		}
	}
}

void AScoreCounter::PrintScoreToRenderComponent()
{
	FString ScoreString("Score: ");
	ScoreString += FString::FromInt(Score);
	if (!bIsEndless)
	{
		ScoreString += FString("/");
		ScoreString += FString::FromInt(ScoreToWin);
	}
	TArray<UTextRenderComponent*> StaticComps;
	GetComponents<UTextRenderComponent>(StaticComps);
	if (StaticComps.IsValidIndex(0))
	{
		UTextRenderComponent* TextComponent = StaticComps[0];
		TextComponent->SetText(ScoreString);
	}
}

void AScoreCounter::CheckIfWon()
{
	if (Score >= ScoreToWin)
	{
		AGrid::SetGridRespondable(false, GetWorld());
		AOpenSettingButton::SetButtonRespondable(false, GetWorld());
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		FStringClassReference MyWidgetClassRef;
		if (GetWorld()->GetName() == "FlabriyaLevel")
		{
			MyWidgetClassRef.SetPath(TEXT("/Game/InGameWidgets/LevelWonPink.LevelWonPink_C"));
		}
		else if (GetWorld()->GetName() == "SirenLevel")
		{
			MyWidgetClassRef.SetPath(TEXT("/Game/InGameWidgets/LevelWonGreen.LevelWonGreen_C"));
		}
		else
		{
			MyWidgetClassRef.SetPath(TEXT("/Game/InGameWidgets/LevelWonBrown.LevelWonBrown_C"));
		}
		APlayerController * MyPlayer = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (UClass* MyWidgetClass = MyWidgetClassRef.TryLoadClass<UUserWidget>())
		{
			UUserWidget* MyWidget = CreateWidget<UUserWidget>(MyPlayer, MyWidgetClass);
			MyWidget->AddToViewport();
		}
	}
}


int32 AScoreCounter::GetScore(UWorld* World)
{
	for (TObjectIterator<AScoreCounter> Itr; Itr; ++Itr)
	{
		if (Itr->GetWorld() != World) {
			continue;
		}
		return Itr->Score;
	}
	return 0;
}