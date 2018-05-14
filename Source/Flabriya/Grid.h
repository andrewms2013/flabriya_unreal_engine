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

	FTileType()
	{
		Probability = 1.0f;
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

	const int TileSpeed = 15;

	virtual void Tick(float DeltaTime) override;

	int SelectTileFromLibrary();

	ATile* CreateTile(class UPaperSprite * TileSprite, FVector SpawnLocation, int SpawnGridAddress, int TileTypeID);

	FVector GetTileCoordinates(int GridAdress);

	void TileMousePressed(ATile* Current);

	bool AreNegihbors(ATile* First, ATile* Second);

	void TryToSwap(ATile* First, ATile* Second);

	void GetMatchedTilesInColoumnOrRow(int Coloumn, bool IsColoumn);

	bool TileCompletesMatchOnStart(int GridAddress, int TileTypeIDs);

	void DestroyTiles();

	void ShiftTiles(int AdressWithNoTile);

	void SpawnNewTiles();

	void CheckForMatches();
};
