// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
#include "Tile.generated.h"
/**
 * 
 */
UCLASS()
class FLABRIYA_API ATile : public APaperSpriteActor
{
	GENERATED_BODY()
public:

	ATile();

	void SetTileSprite(class UPaperSprite * TileSprite);

	virtual void Tick(float DeltaTime) override;
	
	int32 GridAddress;

	int32 TileTypeID;

	int32 TicksToMove = 0;

	bool bIsBomb;

	bool bIsUnmovable;
		
	class AGrid* Grid;

	UFUNCTION()
		void OnSelected(AActor* Target, FKey ButtonPressed);

	UFUNCTION()
		void OnSelectedTouch(ETouchIndex::Type FingerIndex, AActor* TouchedActor);

	FVector MoveVector;
};
