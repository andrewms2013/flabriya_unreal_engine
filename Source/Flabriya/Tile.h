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

	virtual void Tick(float DeltaTime) override;
	
	int GridAddress;

	int TileTypeID;

	int TicksToMove = 0;
		
	UFUNCTION(BlueprintNativeEvent, Category = "Special Game Events")
		void SetTileSprite(class UPaperSprite * TileSprite);
		virtual void SetTileSprite_Implementation(class UPaperSprite * TileSprite);

		class AGrid* Grid;

	UFUNCTION()
		void OnSelected(AActor* Target, FKey ButtonPressed);

	FVector MoveVector;
};
