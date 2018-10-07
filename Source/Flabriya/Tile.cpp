// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Grid.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "PaperSpriteComponent.h"
#include "Engine.h"

ATile::ATile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// We are going to use a Scene Component to base our Tile on. The PaperSpriteActor should have a RenderComponent as the default root, so we are going to attach it to our new root.

	if (RootComponent)
	{
		RootComponent->SetMobility(EComponentMobility::Movable);
	}

	OnClicked.AddUniqueDynamic(this, &ATile::OnSelected);
	OnInputTouchBegin.AddUniqueDynamic(this, &ATile::OnSelectedTouch);
}

void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TicksToMove > 0) 
	{
		FVector Location = GetActorLocation();
		SetActorLocation(Location + MoveVector);
		TicksToMove--;
		if (TicksToMove == 0)
		{
			Grid->TilesAreBeingMoved.Remove(this);
		}
	}
}


void ATile::OnSelected(AActor* Target, FKey ButtonPressed)
{
	if (!(Grid->bSwapHappening) && !(Grid->bFallHappening) && (Grid->bRespondsToClicks))
	{
		Grid->TileMousePressed(this);
	}
}

void ATile::OnSelectedTouch(ETouchIndex::Type FingerIndex, AActor* TouchedActor) {
	FKey ButtonPressed;
	this->OnSelected(TouchedActor, ButtonPressed);
}


void ATile::SetTileSprite(class UPaperSprite * TileSprite)
{
	GetRenderComponent()->SetSprite(TileSprite);
}
