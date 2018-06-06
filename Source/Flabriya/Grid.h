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
		bool bIsBomb;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsUnmovable;

	FTileType()
	{
		Probability = 1.0f;
		bool bIsBomb = false;
		bool bIsUnmovable = false;
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

	UFUNCTION(BlueprintCallable, Category = Initialization)
		void InitFromLoaded(TArray<int32> TilesNumbers);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsEndless;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundBase* OnClick;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundBase* Match;

	void TileMousePressed(ATile* Current);
	ATile* CurrentlySelectedTile;
	TArray<ATile*> TilesToDel;
	TArray<ATile*> TilesAreBeingMoved;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bRespondsToClicks = true;
	
	bool bSwapHappening = false;
	bool bFallHappening = false;
	bool bIsWinnable = true;

	// Vector = 1/30 of TileSize;

	FVector MoveTileUp;
	FVector MoveTileDown;
	FVector MoveTileRight;
	FVector MoveTileLeft;

	// Lower value -> Faster

	const int32 TileSpeed = 7;

	static void SetGridRespondable(bool bIsRespondable, UWorld* World);
	static TArray<ATile*> GetGridTiles (UWorld* World);

private:

	virtual void Tick(float DeltaTime) override;

	const int32 GridHeight = 8;
	const int32 GridWidth = 8;

	ATile* CreateTile(class UPaperSprite * TileSprite, FVector SpawnLocation, int32 SpawnGridAddress, int32 TileTypeID, bool bIsBomb, bool bIsUnmovable);
	int32 SelectTileFromLibrary();
	FVector GetTileCoordinates(int32 GridAdress);

	bool AreValidAndNegihbors(int32 InitialFirstPosition, int32 InitialSecondPosition);
	bool IsValidMove(ATile* First, ATile* Second);
	void SwapTilePositionsInArray(ATile* First, ATile* Second);
	void SwapTilesInGame(ATile* First, ATile* Second);
	void GetMatchedTilesInColoumnOrRow(int32 Coloumn, bool bIsColoumn);
	bool CheckMoveForValidityAndSwapBack(int32 InitialFirstPosition, int32 InitialSecondPosition);

	bool TileCompletesMatchOnStart(int32 GridAddress, int32 TileTypeIDs);
	void ChechForBeingWinnable();

	void SetDestroyingTilesColoutredThanDestroy();
	void DestroyTiles();
	void ShiftTiles(int32 AdressWithNoTile);

	void SpawnNewTiles();
	void CheckForMatches();

	void BombExplode(ATile* Pressed);

	int32 BombsCounter = 0;
	void CopyCurrentToDeleting(TArray<ATile*> CurrentTiles);

};
