// Fill out your copyright notice in the Description page of Project Settings.

#include "Grid.h"
#include "Tile.h"
#include "Timer.h"
#include "Math/UnrealMathUtility.h"
#include "PaperSpriteComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Engine.h"
#include "EngineUtils.h"
#include "ScoreCounter.h"

// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bFallHappening) 
	{
		if (TilesAreBeingMoved.Num() == 0) 
		{
			SpawnNewTiles();
			ChechForBeingWinnable();
			if (!bIsWinnable)
			{
				for (ATile* TileToDel : GameTiles)
				{
					int32 AddressOfTileToDestroy = TileToDel->GridAddress;
					if (TilesAreBeingMoved.Contains(TileToDel))
					{
						TilesAreBeingMoved.Remove(TileToDel);
					}
					GameTiles[AddressOfTileToDestroy] = nullptr;
					TileToDel->Destroy();
				}
				InitGrid();
				bIsWinnable = true;
			}
			CheckForMatches();
			bFallHappening = false;
		}
	}
	if (bSwapHappening) 
	{
		if (TilesAreBeingMoved.Num() == 0)
		{
			SetDestroyingTilesColoutredThanDestroy();
			bSwapHappening = false;
		}
	}

}

ATile* AGrid::CreateTile(class UPaperSprite * TileSprite, FVector SpawnLocation, int32 SpawnGridAddress, int32 TileTypeID, bool bIsBomb, bool bIsUnmovable)
{
	if (TileClass) 
	{
		UWorld* const World = GetWorld();
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		FRotator SpawnRotation(0.0f, 0.0f, 0.0f);
		ATile* const NewTile = World->SpawnActor<ATile>(TileClass, SpawnLocation, SpawnRotation, SpawnParams);
		NewTile->GetRenderComponent()->SetMobility(EComponentMobility::Movable);
		NewTile->TileTypeID = TileTypeID;
		NewTile->SetTileSprite(TileSprite);
		NewTile->GridAddress = SpawnGridAddress;
		NewTile->TicksToMove = 0;
		NewTile->bIsBomb = bIsBomb;
		NewTile->bIsUnmovable = bIsUnmovable;
		NewTile->Grid = this;
		GameTiles[SpawnGridAddress] = NewTile;
		return NewTile;
	}
	return nullptr;
}

void AGrid::InitGrid()
{
	GameTiles.Empty(GridHeight * GridWidth);
	GameTiles.AddZeroed(GameTiles.Max());
	FVector SpawnLocation;
	int32 TileID = 0;
	int32 GridAddress = 0;
	for (int32 Column = 0; Column < GridHeight; Column++)
	{
		for (int32 Row = 0; Row < GridWidth; Row++)
		{
			SpawnLocation = GetTileCoordinates(GridAddress);
			TileID = SelectTileFromLibrary();
			while (TileCompletesMatchOnStart(GridAddress, TileID))
			{
				TileID = SelectTileFromLibrary();
			}
			CreateTile(TileLibrary[TileID].TileSprite, SpawnLocation, GridAddress, TileID, TileLibrary[TileID].bIsBomb, TileLibrary[TileID].bIsUnmovable);
			if (TileLibrary[TileID].bIsBomb) 
			{
				BombsCounter++;
			}
			GridAddress++;
		}
	}
	MoveTileDown = (GetTileCoordinates(1) - GetTileCoordinates(0)) / TileSpeed;
	MoveTileUp = (GetTileCoordinates(0) - GetTileCoordinates(1)) / TileSpeed;
	MoveTileRight = (GetTileCoordinates(8) - GetTileCoordinates(0)) / TileSpeed;
	MoveTileLeft = (GetTileCoordinates(0) - GetTileCoordinates(8)) / TileSpeed;
}

void AGrid::InitFromLoaded(TArray<int32> TilesNumbers) {
	GameTiles.Empty(GridHeight * GridWidth);
	GameTiles.AddZeroed(GameTiles.Max());
	FVector SpawnLocation;
	int32 TileID = 0;
	int32 GridAddress = 0;
	for (int32 Column = 0; Column < GridHeight; Column++)
	{
		for (int32 Row = 0; Row < GridWidth; Row++)
		{
			SpawnLocation = GetTileCoordinates(GridAddress);
			TileID = TilesNumbers[GridAddress];
			CreateTile(TileLibrary[TileID].TileSprite, SpawnLocation, GridAddress, TileID, TileLibrary[TileID].bIsBomb, TileLibrary[TileID].bIsUnmovable);
			if (TileLibrary[TileID].bIsBomb)
			{
				BombsCounter++;
			}
			GridAddress++;
		}
	}
	MoveTileDown = (GetTileCoordinates(1) - GetTileCoordinates(0)) / TileSpeed;
	MoveTileUp = (GetTileCoordinates(0) - GetTileCoordinates(1)) / TileSpeed;
	MoveTileRight = (GetTileCoordinates(8) - GetTileCoordinates(0)) / TileSpeed;
	MoveTileLeft = (GetTileCoordinates(0) - GetTileCoordinates(8)) / TileSpeed;
}

FVector AGrid::GetTileCoordinates(int32 GridAdress)
{
	int32 Column = GridAdress / GridHeight;
	int32 Row = GridAdress % GridWidth;
	FVector SpawnCoordinates = GetActorLocation();
	if (Column < 4)
	{
		float XOffset = 37.5 + 75.0 * (3 - Column);
		SpawnCoordinates.X -= XOffset;
	}
	else 
	{
		float XOffset = 37.5 + 75.0 * (Column - 4);
		SpawnCoordinates.X += XOffset;
	}
	if(Row < 4)
	{
		float ZOffset = 37.5 + 75.0 * (3 - Row);
		SpawnCoordinates.Z += ZOffset;
	}
	else 
	{
		float ZOffset = 37.5 + 75.0 * (Row - 4);
		SpawnCoordinates.Z -= ZOffset;
	}
	return SpawnCoordinates;
}

int32 AGrid::SelectTileFromLibrary()
{
	float ChosenNumber = 0.0f;
	for (int32 TileID = 0; TileID < TileLibrary.Num(); TileID++)
	{
		ChosenNumber += TileLibrary[TileID].Probability;
	}
	ChosenNumber = FMath::FRandRange(0.0f, ChosenNumber);
	float CurrentNumber = 0.0f;
	for (int32 TileID = 0; TileID < TileLibrary.Num(); TileID++) 
	{
		CurrentNumber += TileLibrary[TileID].Probability;
		if (ChosenNumber <= CurrentNumber)
		{
			return TileID;
		}
	}
	return 0;
}

void AGrid::TileMousePressed(ATile* Pressed)
{
	UGameplayStatics::PlaySound2D(GetWorld(), OnClick);
	if (Pressed->bIsUnmovable || Pressed->bIsBomb) 
	{
		if (CurrentlySelectedTile != nullptr)
		{
			CurrentlySelectedTile->SetTileSprite(TileLibrary[CurrentlySelectedTile->TileTypeID].TileSprite);
			CurrentlySelectedTile = nullptr;
		}
		if (Pressed->bIsUnmovable)
		{
			FStringClassReference MyWidgetClassRef;
			MyWidgetClassRef.SetPath(TEXT("/Game/InGameWidgets/TileIsUnmovable.TileIsUnmovable_C"));
			if (UClass* MyWidgetClass = MyWidgetClassRef.TryLoadClass<UUserWidget>()) 
			{
				APlayerController * MyPlayer = UGameplayStatics::GetPlayerController(GetWorld(), 0);
				UUserWidget* MyWidget = CreateWidget<UUserWidget>(MyPlayer, MyWidgetClass);
				MyWidget->AddToViewport();
			}
		}
		if (Pressed->bIsBomb) 
		{
			BombExplode(Pressed);
		}
		return;
	}
	if (CurrentlySelectedTile != nullptr) 
	{
		if (AreValidAndNegihbors(Pressed->GridAddress, CurrentlySelectedTile->GridAddress))
		{
			if (IsValidMove(CurrentlySelectedTile, Pressed)) {
				SwapTilesInGame(CurrentlySelectedTile, Pressed);
			}
			else {
				SwapTilePositionsInArray(CurrentlySelectedTile, Pressed);
			}
		}
		CurrentlySelectedTile->SetTileSprite(TileLibrary[CurrentlySelectedTile->TileTypeID].TileSprite);
		CurrentlySelectedTile = nullptr;
		return;
	}
	Pressed->SetTileSprite(TileLibrary[Pressed->TileTypeID].TilePressedSprite);
	CurrentlySelectedTile = Pressed;
}

bool AGrid::AreValidAndNegihbors(int32 InitialFirstPosition, int32 InitialSecondPosition)
{
	if (InitialFirstPosition < 64 && InitialFirstPosition >= 0 && InitialSecondPosition < 64 && InitialSecondPosition >= 0) 
	{
		int32 Offset = FMath::Abs(InitialFirstPosition - InitialSecondPosition);
		if (Offset == 1 || Offset == 8)
		{
			return true;
		}
	}
	return false;
}

bool AGrid::IsValidMove(ATile* First, ATile* Second) 
{
	int32 InitialFirstPosition = First->GridAddress;
	int32 InitialSecondPosition = Second->GridAddress;
	SwapTilePositionsInArray(First, Second);
	if (FMath::Abs(InitialFirstPosition - InitialSecondPosition) == 1) 
	{
		int32 RowFirst = InitialFirstPosition % 8;
		int32 RowSecond = InitialSecondPosition % 8;
		int32 Coloumn = InitialFirstPosition / 8;
		GetMatchedTilesInColoumnOrRow(RowFirst, false);
		GetMatchedTilesInColoumnOrRow(RowSecond, false);
		GetMatchedTilesInColoumnOrRow(Coloumn, true);
	}
	else
	{
		int32 Row = InitialFirstPosition % 8;
		int32 ColoumnFirst = InitialFirstPosition / 8;
		int32 ColoumnSecond = InitialSecondPosition / 8;
		GetMatchedTilesInColoumnOrRow(Row, false);
		GetMatchedTilesInColoumnOrRow(ColoumnFirst, true);
		GetMatchedTilesInColoumnOrRow(ColoumnSecond, true);
	}
	return(TilesToDel.Num() != 0);
}

void AGrid::SwapTilesInGame(ATile* First, ATile* Second) {
	int32 Offset = First->GridAddress - Second->GridAddress;
	TilesAreBeingMoved.Add(First);
	TilesAreBeingMoved.Add(Second);
	if (Offset == 1)
	{
		First->MoveVector = MoveTileDown;
		Second->MoveVector = MoveTileUp;
	}
	else if (Offset == -1)
	{
		First->MoveVector = MoveTileUp;
		Second->MoveVector = MoveTileDown;
	}
	else if (Offset == 8)
	{
		First->MoveVector = MoveTileRight;
		Second->MoveVector = MoveTileLeft;
	}
	else
	{
		First->MoveVector = MoveTileLeft;
		Second->MoveVector = MoveTileRight;
	}
	First->TicksToMove = TileSpeed;
	Second->TicksToMove = TileSpeed;
	bSwapHappening = true;
}

void AGrid::SwapTilePositionsInArray(ATile* First, ATile* Second) 
{
	int32 InitialFirstPosition = First->GridAddress;
	int32 InitialSecondPosition = Second->GridAddress;
	First->GridAddress = InitialSecondPosition;
	Second->GridAddress = InitialFirstPosition;
	GameTiles[InitialFirstPosition] = Second;
	GameTiles[InitialSecondPosition] = First;
}

void AGrid::CheckForMatches()
{
	for (int32 i = 0; i < GridWidth; i++)
	{
		GetMatchedTilesInColoumnOrRow(i, false);
	}
	for (int32 i = 0; i < GridHeight; i++) 
	{
		GetMatchedTilesInColoumnOrRow(i, true);
	}
	if (TilesToDel.Num() != 0)
	{
		SetDestroyingTilesColoutredThanDestroy();
	}
}

void AGrid::GetMatchedTilesInColoumnOrRow(int32 Index, bool bIsColoumn) 
{
	TArray<ATile*> CurrentTiles;
	int32 SameTilesCounter = 0;
	int32 StartPosition = Index * 8;
	int32 EndPosition = (Index + 1) * 8;
	int32 Offset = 1;
	if (!bIsColoumn)
	{
		StartPosition = Index;
		EndPosition = 64;
		Offset = 8;
	}
	for (int32 i = StartPosition; i < EndPosition; i += Offset) 
	{
		if (GameTiles[i] != nullptr) 
		{
			if (GameTiles[i]->bIsBomb || GameTiles[i]->bIsUnmovable) 
			{
				if (SameTilesCounter > 2)
				{
					CopyCurrentToDeleting(CurrentTiles);
				}
				CurrentTiles.Empty();
				SameTilesCounter = 0;
				continue;
			}
			if (SameTilesCounter == 0) 
			{
				CurrentTiles.Add(GameTiles[i]);
				SameTilesCounter++;
			}
			else 
			{
				if (CurrentTiles[SameTilesCounter - 1]->TileTypeID == GameTiles[i]->TileTypeID) 
				{
					CurrentTiles.Add(GameTiles[i]);
					SameTilesCounter++;
				}
				else
				{
					if (SameTilesCounter > 2)
					{
						CopyCurrentToDeleting(CurrentTiles);
					}
					CurrentTiles.Empty();
					CurrentTiles.Add(GameTiles[i]);
					SameTilesCounter = 1;
				}
			}
		}
	}
	if (SameTilesCounter > 2)
	{
		for (ATile* TileToDel : CurrentTiles) 
		{
			if (!TilesToDel.Contains(TileToDel)) 
			{
				TilesToDel.Add(TileToDel);
			}
		}
	}
}

bool AGrid::TileCompletesMatchOnStart(int32 GridAddress, int32 TileTypeID)
{
	if (GridAddress % 8 > 1) 
	{
		if (GameTiles[GridAddress - 2]->TileTypeID == GameTiles[GridAddress - 1]->TileTypeID &&
			GameTiles[GridAddress - 1]->TileTypeID == TileTypeID) 
		{
			return true;
		}
	}
	if (GridAddress > 15)
	{
		if (GameTiles[GridAddress - 16]->TileTypeID == GameTiles[GridAddress - 8]->TileTypeID && 
			GameTiles[GridAddress - 8]->TileTypeID  == TileTypeID) 
		{
			return true;
		}
	}
	return false;
}

void AGrid::SetDestroyingTilesColoutredThanDestroy()
{
	bRespondsToClicks = false;
	UGameplayStatics::PlaySound2D(GetWorld(), Match);
	for (ATile* TileToDel : TilesToDel)
	{
		TileToDel->SetTileSprite(TileLibrary[TileToDel->TileTypeID].TilePressedSprite);
		if (!TileToDel->bIsBomb) {
			AScoreCounter::AddOnePoint(GetWorld());
		}
	}
	if (bIsEndless)
	{
		for (TObjectIterator<ATimer> Itr; Itr; ++Itr)
		{
			if (Itr->GetWorld() != GetWorld()) 
			{
				continue;
			}
			Itr->TimeLeftSeconds++;
		}
		FStringClassReference MyWidgetClassRef;
		MyWidgetClassRef.SetPath(TEXT("/Game/InGameWidgets/PlusOneSec.PlusOneSec_C"));
		if (UClass* MyWidgetClass = MyWidgetClassRef.TryLoadClass<UUserWidget>())
		{
			APlayerController * MyPlayer = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			UUserWidget* MyWidget = CreateWidget<UUserWidget>(MyPlayer, MyWidgetClass);
			MyWidget->AddToViewport();
		}
	}
	FTimerHandle DestroyTimerHandle;
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AGrid::DestroyTiles, 0.1f, false);
}

void AGrid::DestroyTiles()
{
	for (ATile* TileToDel : TilesToDel)
	{
		int32 AddressOfTileToDestroy = TileToDel->GridAddress;
		if (TilesAreBeingMoved.Contains(TileToDel))
		{
			TilesAreBeingMoved.Remove(TileToDel);
		}
		GameTiles[AddressOfTileToDestroy] = nullptr;
		if (TileToDel->bIsBomb) 
		{
			BombsCounter--;
		}
		TileToDel->Destroy();
		ShiftTiles(AddressOfTileToDestroy);
	}
	TilesToDel.Empty();
	bRespondsToClicks = true;
}


void AGrid::ShiftTiles(int32 AddressWithNoTile)
{
	while (AddressWithNoTile % 8 != 0)
	{
		int32 AddressOfUpperTile = AddressWithNoTile - 1;
		if (GameTiles[AddressOfUpperTile] != nullptr) 
		{
			if (!TilesAreBeingMoved.Contains(GameTiles[AddressOfUpperTile])) 
			{
				TilesAreBeingMoved.Add(GameTiles[AddressOfUpperTile]);
			}
			GameTiles[AddressOfUpperTile]->MoveVector = MoveTileDown;
			GameTiles[AddressOfUpperTile]->TicksToMove += TileSpeed;
			GameTiles[AddressOfUpperTile]->GridAddress = AddressWithNoTile;
		}
		GameTiles[AddressWithNoTile] = GameTiles[AddressOfUpperTile];
		AddressWithNoTile--;
	}
	GameTiles[AddressWithNoTile] = nullptr;
	bFallHappening = true;
}


void AGrid::SpawnNewTiles()
{
	for (int32 i = 0; i < 63; i++)
	{
		if (GameTiles[i] == nullptr)
		{
			int32 TileID = SelectTileFromLibrary();
			FVector SpawnLocation = GetTileCoordinates(i);
			CreateTile(TileLibrary[TileID].TileSprite, SpawnLocation, i, TileID, TileLibrary[TileID].bIsBomb, TileLibrary[TileID].bIsUnmovable);
			if (TileLibrary[TileID].bIsBomb) 
			{
				BombsCounter++;
			}
		}
	}
}

void AGrid::BombExplode(ATile* Pressed) {
	TArray<int32> Coordinates = { 1, -1, 8, -8 };
	for (int32 &i : Coordinates) {
		int32 AddressToDel = Pressed->GridAddress + i;
		if (AddressToDel >= 0) 
		{
			if (FMath::Abs(i) == 1 && AddressToDel / 8 == (Pressed->GridAddress) / 8)
			{
				if (!TilesToDel.Contains(GameTiles[AddressToDel]))
				{
					TilesToDel.Add(GameTiles[AddressToDel]);
				}
			}
			if (FMath::Abs(i) == 8 && AddressToDel < 64)
			{
				if (!TilesToDel.Contains(GameTiles[AddressToDel]))
				{
					TilesToDel.Add(GameTiles[AddressToDel]);
				}
			}
		}
	}
	if (!TilesToDel.Contains(Pressed))
	{
		TilesToDel.Add(Pressed);
	}
	SetDestroyingTilesColoutredThanDestroy();
}

void AGrid::CopyCurrentToDeleting(TArray<ATile*> CurrentTiles) {
	for (ATile* TileToDel : CurrentTiles)
	{
		if (!TilesToDel.Contains(TileToDel))
		{
			TilesToDel.Add(TileToDel);
		}
	}
}

void AGrid::ChechForBeingWinnable() {
	if (BombsCounter) 
	{
		return;
	}
	for (ATile* Tile : GameTiles) {
		int32 TilePosition = Tile->GridAddress;
		if (CheckMoveForValidityAndSwapBack(TilePosition, TilePosition + 1) ||
			CheckMoveForValidityAndSwapBack(TilePosition, TilePosition - 1) ||
			CheckMoveForValidityAndSwapBack(TilePosition, TilePosition + 8) ||
			CheckMoveForValidityAndSwapBack(TilePosition, TilePosition - 8))
		{
			return;
		}
	}
	bIsWinnable = false;
}

bool AGrid::CheckMoveForValidityAndSwapBack(int32 InitialFirstPosition, int32 InitialSecondPosition)
{
	if (AreValidAndNegihbors(InitialFirstPosition, InitialSecondPosition))
	{
		if (GameTiles[InitialFirstPosition]->bIsBomb || GameTiles[InitialSecondPosition]->bIsBomb ||
			GameTiles[InitialFirstPosition]->bIsUnmovable || GameTiles[InitialSecondPosition]->bIsUnmovable) 
		{
			return false;
		}
		if (IsValidMove(GameTiles[InitialFirstPosition], GameTiles[InitialSecondPosition]))
		{
			TilesToDel.Empty();
			SwapTilePositionsInArray(GameTiles[InitialFirstPosition], GameTiles[InitialSecondPosition]);
			return true;
		}
		else
		{
			SwapTilePositionsInArray(GameTiles[InitialFirstPosition], GameTiles[InitialSecondPosition]);
		}
	}
	return false;
}

void AGrid::SetGridRespondable(bool bIsRespondable, UWorld* World)
{
	for (TObjectIterator<AGrid> Itr; Itr; ++Itr)
	{
		if (Itr->GetWorld() != World) {
			continue;
		}
		Itr->bRespondsToClicks = bIsRespondable;
	}
}

TArray<ATile*> AGrid::GetGridTiles(UWorld* World)
{
	for (TObjectIterator<AGrid> Itr; Itr; ++Itr)
	{
		if (Itr->GetWorld() != World) {
			continue;
		}
		return Itr->GameTiles;
	}
	TArray<ATile*> Tiles;
	return Tiles;
}