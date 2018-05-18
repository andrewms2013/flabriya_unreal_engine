// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.h"
#include "Grid.generated.h"

USTRUCT(BlueprintType)
struct FTileType
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Probability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPaperSprite * TileSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPaperSprite * TilePressedSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsBomb;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsUnmovable;

	FTileType()
	{
		Probability = 1.0f;
		bool IsBomb = false;
		bool IsUnmovable = false;
	}
};

UCLASS()
class FLABRIYA_API AGrid : public AActor
{
	GENERATED_BODY()
	
public:	

	AGrid();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<ATile*> GameTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTileType> TileLibrary;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ATile> TileClass;

	UFUNCTION(BlueprintCallable, Category = Initialization)
		void InitGrid();

	ATile* CurrentlySelectedTile;

	TArray<ATile*> TilesToDel;

	TArray<ATile*> TilesAreBeingMoved;

	bool SwapHappening = false;

	bool FallHappening = false;

	// Vector = 1/30 of TileSize;

	FVector MoveTileUp;
	FVector MoveTileDown;
	FVector MoveTileRight;
	FVector MoveTileLeft;

	virtual void Tick(float DeltaTime) override;

	int SelectTileFromLibrary();

	ATile* CreateTile(class UPaperSprite * TileSprite, FVector SpawnLocation, int SpawnGridAddress, int TileTypeID, bool IsBomb, bool IsUnmovable);

	FVector GetTileCoordinates(int GridAdress);

	void TileMousePressed(ATile* Current);

	bool AreValidAndNegihbors(int InitialFirstPosition, int InitialSecondPosition);

	void SwapTilePositionsInArray(ATile* First, ATile* Second);

	bool IsValidMove(ATile* First, ATile* Second);

	void SwapTilesInGame(ATile* First, ATile* Second);

	void GetMatchedTilesInColoumnOrRow(int Coloumn, bool IsColoumn);

	bool TileCompletesMatchOnStart(int GridAddress, int TileTypeIDs);

	void DestroyTiles();

	void ShiftTiles(int AdressWithNoTile);

	void SpawnNewTiles();

	void CheckForMatches();

	// Lower value -> Faster

	const int TileSpeed = 7;

	void BombExplode(ATile* Pressed);

	bool IsWinnable();

	bool CheckMoveForValidityAndSwapBack(int InitialFirstPosition, int InitialSecondPosition);

private:

	int BombsCounter = 0;

	void CopyCurrentToDeleting(TArray<ATile*> CurrentTiles);
};
