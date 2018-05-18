// Fill out your copyright notice in the Description page of Project Settings.

#include "Grid.h"
#include "Tile.h"
#include "Math/UnrealMathUtility.h"
#include "PaperSpriteComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Engine.h"


// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FallHappening) 
	{
		if (TilesAreBeingMoved.Num() == 0) 
		{
			FallHappening = false;
			SpawnNewTiles();
			if (!IsWinnable()) {
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString("Is Unwinnable"));
			}
			CheckForMatches();
		}
	}
	if (SwapHappening) 
	{
		if (TilesAreBeingMoved.Num() == 0)
		{
			SwapHappening = false;
			DestroyTiles();
		}
	}

}

ATile* AGrid::CreateTile(class UPaperSprite * TileSprite, FVector SpawnLocation, int SpawnGridAddress, int TileTypeID, bool IsBomb, bool IsUnmovable)
{
	if (TileClass) {
		UWorld* const World = GetWorld();
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FRotator SpawnRotation(0.0f, 0.0f, 0.0f);
		ATile* const NewTile = World->SpawnActor<ATile>(TileClass, SpawnLocation, SpawnRotation, SpawnParams);
		NewTile->GetRenderComponent()->SetMobility(EComponentMobility::Movable);
		NewTile->TileTypeID = TileTypeID;
		NewTile->SetTileSprite(TileSprite);
		NewTile->GridAddress = SpawnGridAddress;
		NewTile->TicksToMove = 0;
		NewTile->IsBomb = IsBomb;
		NewTile->IsUnmovable = IsUnmovable;
		NewTile->Grid = this;
		GameTiles[SpawnGridAddress] = NewTile;
		return NewTile;
	}
	return nullptr;
}

void AGrid::InitGrid()
{
	GameTiles.Empty(64);
	GameTiles.AddZeroed(GameTiles.Max());
	FVector SpawnLocation;
	int TileID = 0;
	int GridAddress = 0;
	for (int Column = 0; Column < 8; Column++)
	{
		for (int Row = 0; Row < 8; Row++)
		{
			SpawnLocation = GetTileCoordinates(GridAddress);
			TileID = SelectTileFromLibrary();
			while (TileCompletesMatchOnStart(GridAddress, TileID) || TileLibrary[TileID].IsBomb || TileLibrary[TileID].IsUnmovable) {
				TileID = SelectTileFromLibrary();
			}
			CreateTile(TileLibrary[TileID].TileSprite, SpawnLocation, GridAddress, TileID, TileLibrary[TileID].IsBomb, TileLibrary[TileID].IsUnmovable);
			GridAddress++;
		}
	}
	MoveTileDown = (GetTileCoordinates(1) - GetTileCoordinates(0)) / TileSpeed;
	MoveTileUp = (GetTileCoordinates(0) - GetTileCoordinates(1)) / TileSpeed;
	MoveTileRight = (GetTileCoordinates(8) - GetTileCoordinates(0)) / TileSpeed;
	MoveTileLeft = (GetTileCoordinates(0) - GetTileCoordinates(8)) / TileSpeed;
}

FVector AGrid::GetTileCoordinates(int GridAdress)
{
	int Column = GridAdress / 8;
	int Row = GridAdress % 8;
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

int AGrid::SelectTileFromLibrary()
{
	float ChosenNumber = 0.0f;
	for (int TileID = 0; TileID < TileLibrary.Num(); TileID++)
	{
		ChosenNumber += TileLibrary[TileID].Probability;
	}
	ChosenNumber = FMath::FRandRange(0.0f, ChosenNumber);
	float CurrentNumber = 0.0f;
	for (int TileID = 0; TileID < TileLibrary.Num(); TileID++) 
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
	if (Pressed->IsUnmovable || Pressed->IsBomb) 
	{
		if (CurrentlySelectedTile != nullptr)
		{
			CurrentlySelectedTile->SetTileSprite(TileLibrary[CurrentlySelectedTile->TileTypeID].TileSprite);
			CurrentlySelectedTile = nullptr;
		}
		if (Pressed->IsBomb) 
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

bool AGrid::AreValidAndNegihbors(int InitialFirstPosition, int InitialSecondPosition)
{
	if (InitialFirstPosition < 64 && InitialFirstPosition >= 0 && InitialSecondPosition < 64 && InitialSecondPosition >= 0) 
	{
		int Offset = FMath::Abs(InitialFirstPosition - InitialSecondPosition);
		if (Offset == 1 || Offset == 8)
		{
			return true;
		}
	}
	return false;
}

bool AGrid::IsValidMove(ATile* First, ATile* Second) 
{
	int InitialFirstPosition = First->GridAddress;
	int InitialSecondPosition = Second->GridAddress;
	SwapTilePositionsInArray(First, Second);
	if (FMath::Abs(InitialFirstPosition - InitialSecondPosition) == 1) 
	{
		int RowFirst = InitialFirstPosition % 8;
		int RowSecond = InitialSecondPosition % 8;
		int Coloumn = InitialFirstPosition / 8;
		GetMatchedTilesInColoumnOrRow(RowFirst, false);
		GetMatchedTilesInColoumnOrRow(RowSecond, false);
		GetMatchedTilesInColoumnOrRow(Coloumn, true);
	}
	else
	{
		int Row = InitialFirstPosition % 8;
		int ColoumnFirst = InitialFirstPosition / 8;
		int ColoumnSecond = InitialSecondPosition / 8;
		GetMatchedTilesInColoumnOrRow(Row, false);
		GetMatchedTilesInColoumnOrRow(ColoumnFirst, true);
		GetMatchedTilesInColoumnOrRow(ColoumnSecond, true);
	}
	return(TilesToDel.Num() != 0);
}

void AGrid::SwapTilesInGame(ATile* First, ATile* Second) {
	int Offset = First->GridAddress - Second->GridAddress;
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
	SwapHappening = true;
}

void AGrid::SwapTilePositionsInArray(ATile* First, ATile* Second) 
{
	int InitialFirstPosition = First->GridAddress;
	int InitialSecondPosition = Second->GridAddress;
	First->GridAddress = InitialSecondPosition;
	Second->GridAddress = InitialFirstPosition;
	GameTiles[InitialFirstPosition] = Second;
	GameTiles[InitialSecondPosition] = First;
}

void AGrid::CheckForMatches()
{
	for (int i = 0; i < 8; i++)
	{
		GetMatchedTilesInColoumnOrRow(i, false);
	}
	for (int i = 0; i < 8; i++) 
	{
		GetMatchedTilesInColoumnOrRow(i, true);
	}
	if (TilesToDel.Num() != 0)
	{
		DestroyTiles();
	}
}

void AGrid::GetMatchedTilesInColoumnOrRow(int Index, bool IsColoumn) 
{
	TArray<ATile*> CurrentTiles;
	int SameTilesCounter = 0;
	int StartPosition = Index * 8;
	int EndPosition = (Index + 1) * 8;
	int Offset = 1;
	if (!IsColoumn)
	{
		StartPosition = Index;
		EndPosition = 64;
		Offset = 8;
	}
	for (int i = StartPosition; i < EndPosition; i += Offset) 
	{
		if (GameTiles[i] != nullptr) 
		{
			if (GameTiles[i]->IsBomb || GameTiles[i]->IsUnmovable) 
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

bool AGrid::TileCompletesMatchOnStart(int GridAddress, int TileTypeID)
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

void AGrid::DestroyTiles()
{
	for (ATile* TileToDel : TilesToDel) 
	{
		int AddressOfTileToDestroy = TileToDel->GridAddress;
		if (TilesAreBeingMoved.Contains(TileToDel)) 
		{
			TilesAreBeingMoved.Remove(TileToDel);
		}
		GameTiles[AddressOfTileToDestroy] = nullptr;
		if (TileToDel->IsBomb) {
			BombsCounter--;
		}
		TileToDel->Destroy();
		ShiftTiles(AddressOfTileToDestroy);
	}
	TilesToDel.Empty();
}

void AGrid::ShiftTiles(int AddressWithNoTile)
{
	while (AddressWithNoTile % 8 != 0)
	{
		int AddressOfUpperTile = AddressWithNoTile - 1;
		if (GameTiles[AddressOfUpperTile] != nullptr) {
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
	FallHappening = true;
}


void AGrid::SpawnNewTiles()
{
	for (int i = 0; i < 63; i++)
	{
		if (GameTiles[i] == nullptr)
		{
			int TileID = SelectTileFromLibrary();
			FVector SpawnLocation = GetTileCoordinates(i);
			CreateTile(TileLibrary[TileID].TileSprite, SpawnLocation, i, TileID, TileLibrary[TileID].IsBomb, TileLibrary[TileID].IsUnmovable);
			if (TileLibrary[TileID].IsBomb) {
				BombsCounter++;
			}
		}
	}
}

void AGrid::BombExplode(ATile* Pressed) {
	TArray<int> Coordinates = { 1, -1, 8, -8 };
	for (int &i : Coordinates) {
		int AddressToDel = Pressed->GridAddress + i;
		if (AddressToDel >= 0) {
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
	DestroyTiles();
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

bool AGrid::IsWinnable() {
	if (BombsCounter) {
		return true;
	}
	for (ATile* Tile : GameTiles) {
		int TilePosition = Tile->GridAddress;
		if (CheckMoveForValidityAndSwapBack(TilePosition, TilePosition + 1) ||
			CheckMoveForValidityAndSwapBack(TilePosition, TilePosition - 1) ||
			CheckMoveForValidityAndSwapBack(TilePosition, TilePosition + 8) ||
			CheckMoveForValidityAndSwapBack(TilePosition, TilePosition - 8))
			return true;
	}
	return false;
}

bool AGrid::CheckMoveForValidityAndSwapBack(int InitialFirstPosition, int InitialSecondPosition) {
	if (AreValidAndNegihbors(InitialFirstPosition, InitialSecondPosition))
	{
		if (GameTiles[InitialFirstPosition]->IsBomb || GameTiles[InitialSecondPosition]->IsBomb ||
			GameTiles[InitialFirstPosition]->IsUnmovable || GameTiles[InitialSecondPosition]->IsUnmovable)
			return false;
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