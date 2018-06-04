// Fill out your copyright notice in the Description page of Project Settings.

#include "FlabryaGameInstance.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "FlabriyaSaveProgress.h"
#include "FlabriyaSaveGame.h"
#include "ScoreCounter.h"
#include "Grid.h"
#include "Timer.h"
#include "Engine.h"
#include "EngineUtils.h"

UFlabryaGameInstance::UFlabryaGameInstance()
{

}

void UFlabryaGameInstance::SaveGame() 
{
	FString Level = this->GetWorld()->GetName();
	TArray<ATile*> TilesToSave = AGrid::GetGridTiles(GetWorld());
	TArray<int32> TilesNumbers;
	for (ATile* Tile : TilesToSave) {
		TilesNumbers.Add(Tile->TileTypeID);
	}
	int32 TimeToSave = ATimer::GetTimerLeftSeconds(GetWorld());
	int32 Score = AScoreCounter::GetScore(GetWorld());
	UFlabriyaSaveGame* SaveGameInstance = Cast<UFlabriyaSaveGame>(UGameplayStatics::CreateSaveGameObject(UFlabriyaSaveGame::StaticClass()));
	SaveGameInstance->LevelName = Level;
	SaveGameInstance->GameTiles = TilesNumbers;
	SaveGameInstance->SecondsLeft = TimeToSave;
	SaveGameInstance->Score = Score;
	FString SlotName = PlayerName + FString("Save");
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, 0);
}

void UFlabryaGameInstance::SetForLoadAndOpenLevel()
{
	UFlabriyaSaveGame* LoadGameInstance = Cast<UFlabriyaSaveGame>(UGameplayStatics::CreateSaveGameObject(UFlabriyaSaveGame::StaticClass()));
	FString SlotName = PlayerName + FString("Save");
	if (UGameplayStatics::LoadGameFromSlot(SlotName, 0)) {
		LoadGameInstance = Cast<UFlabriyaSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		FString Level = LoadGameInstance->LevelName;
		bIsLoads = true;
		UGameplayStatics::OpenLevel(GetWorld(), FName(*Level));
	}
	else
	{
		FStringClassReference MyWidgetClassRef;
		if (GetWorld()->GetName() == "FlabriyaLevel" || GetWorld()->GetName() == "EndlessLevel")
		{
			MyWidgetClassRef.SetPath(TEXT("/Game/InGameWidgets/LoadFailedPink.LoadFailedPink_C"));
		}
		else if (GetWorld()->GetName() == "MainMenu" || GetWorld()->GetName() == "SirenLevel")
		{
			MyWidgetClassRef.SetPath(TEXT("/Game/InGameWidgets/LoadFailedGreen.LoadFailedGreen_C"));
		}
		else
		{
			MyWidgetClassRef.SetPath(TEXT("/Game/InGameWidgets/LoadFailedBrown.LoadFailedBrown_C"));
		}
		APlayerController * MyPlayer = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (UClass* MyWidgetClass = MyWidgetClassRef.TryLoadClass<UUserWidget>())
		{
			UUserWidget* MyWidget = CreateWidget<UUserWidget>(MyPlayer, MyWidgetClass);
			MyWidget->AddToViewport();
		}
	}
}

void UFlabryaGameInstance::LoadGame() {
	UFlabriyaSaveGame* LoadGameInstance = Cast<UFlabriyaSaveGame>(UGameplayStatics::CreateSaveGameObject(UFlabriyaSaveGame::StaticClass()));
	FString SlotName = PlayerName + FString("Save");
	LoadGameInstance = Cast<UFlabriyaSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	TArray<int32> TilesNumbers = LoadGameInstance->GameTiles;
	for (TObjectIterator<AGrid> Itr; Itr; ++Itr)
	{
		if (Itr->GetWorld() != GetWorld())
		{
			continue;
		}
		Itr->InitFromLoaded(TilesNumbers);
	}
	for (TObjectIterator<AScoreCounter> Itr; Itr; ++Itr)
	{
		if (Itr->GetWorld() != GetWorld())
		{
			continue;
		}
		Itr->Score = LoadGameInstance->Score;
		Itr->InitScoreCounter();
	}
	for (TObjectIterator<ATimer> Itr; Itr; ++Itr)
	{
		if (Itr->GetWorld() != GetWorld())
		{
			continue;
		}
		Itr->TimeLeftSeconds = LoadGameInstance->SecondsLeft;
		Itr->InitTimer();
	}
	bIsLoads = false;
}

void UFlabryaGameInstance::EnterNameAndOpenMenu()
{
	if (!bIsNameWritten)
	{
		FStringClassReference MyWidgetClassRef(TEXT("/Game/InGameWidgets/EnterName.EnterName_C"));
		APlayerController * MyPlayer = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (UClass* MyWidgetClass = MyWidgetClassRef.TryLoadClass<UUserWidget>())
		{
			UUserWidget* MyWidget = CreateWidget<UUserWidget>(MyPlayer, MyWidgetClass);
			MyWidget->AddToViewport();
			bIsNameWritten = true;
		}
	}
	else
	{
		FStringClassReference MyWidgetClassRef(TEXT("/Game/MainMenu/MainMenuWidget.MainMenuWidget_C"));
		APlayerController * MyPlayer = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (UClass* MyWidgetClass = MyWidgetClassRef.TryLoadClass<UUserWidget>())
		{
			UUserWidget* MyWidget = CreateWidget<UUserWidget>(MyPlayer, MyWidgetClass);
			MyWidget->AddToViewport();
		}
	}
}

void UFlabryaGameInstance::SaveProgress(int32 LvlWon)
{
	UFlabriyaSaveProgress* SaveGameInstance = Cast<UFlabriyaSaveProgress>(UGameplayStatics::CreateSaveGameObject(UFlabriyaSaveProgress::StaticClass()));
	SaveGameInstance->LevelsWon = LvlWon;
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, PlayerName, 0);
}

void UFlabryaGameInstance::LoadProgress()
{
	UFlabriyaSaveProgress* LoadGameInstance = Cast<UFlabriyaSaveProgress>(UGameplayStatics::CreateSaveGameObject(UFlabriyaSaveProgress::StaticClass()));
	if (UGameplayStatics::LoadGameFromSlot(PlayerName, 0)) {
		LoadGameInstance = Cast<UFlabriyaSaveProgress>(UGameplayStatics::LoadGameFromSlot(PlayerName, 0));
		LevelsWon = LoadGameInstance->LevelsWon;
	}
	else
	{
		LevelsWon = 0;
	}
}