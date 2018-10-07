// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenSettingButton.h"
#include "PaperSpriteComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Engine.h"
#include "Grid.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"
#include "EngineUtils.h"

AOpenSettingButton::AOpenSettingButton() 
{
	PrimaryActorTick.bCanEverTick = false;

	bRespondsToClicks = true;

	if (RootComponent)
	{
		RootComponent->SetMobility(EComponentMobility::Movable);
	}

	OnClicked.AddUniqueDynamic(this, &AOpenSettingButton::OnSelected);
	OnBeginCursorOver.AddUniqueDynamic(this, &AOpenSettingButton::OnMouseOver);
	OnEndCursorOver.AddUniqueDynamic(this, &AOpenSettingButton::OnMouseOverFinished);
	OnInputTouchBegin.AddUniqueDynamic(this, &AOpenSettingButton::OnSelectedTouch);
}

void AOpenSettingButton::OnSelected(AActor* Target, FKey ButtonPressed)
{
	if (bRespondsToClicks) 
	{
		AGrid * Grid = nullptr;
		UGameplayStatics::PlaySound2D(GetWorld(), OnClick);
		for (TObjectIterator<AGrid> Itr; Itr; ++Itr)
		{
			if (Itr->GetWorld() != GetWorld()) {
				continue;
			}
			Grid = (*Itr);
		}
		if (Grid && !(Grid->bSwapHappening) && !(Grid->bFallHappening) && (Grid->bRespondsToClicks)) {
			FStringClassReference MyWidgetClassRef;
			if (GetWorld()->GetName() == "FlabriyaLevel" || GetWorld()->GetName() == "EndlessLevel")
			{
				MyWidgetClassRef.SetPath(TEXT("/Game/InGameWidgets/SettingsMenuPink.SettingsMenuPink_C"));
			}
			else if (GetWorld()->GetName() == "SirenLevel")
			{
				MyWidgetClassRef.SetPath(TEXT("/Game/InGameWidgets/SettingsMenuGreen.SettingsMenuGreen_C"));
			}
			else
			{
				MyWidgetClassRef.SetPath(TEXT("/Game/InGameWidgets/SettingsMenuBrown.SettingsMenuBrown_C"));
			}
			APlayerController * MyPlayer = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (UClass* MyWidgetClass = MyWidgetClassRef.TryLoadClass<UUserWidget>())
			{
				AGrid::SetGridRespondable(false, GetWorld());
				AOpenSettingButton::SetButtonRespondable(false, GetWorld());
				UGameplayStatics::SetGamePaused(GetWorld(), true);
				UUserWidget* MyWidget = CreateWidget<UUserWidget>(MyPlayer, MyWidgetClass);
				MyWidget->AddToViewport();
			}
		}
	}
}

void AOpenSettingButton::OnSelectedTouch(ETouchIndex::Type FingerIndex, AActor* TouchedActor) {
	FKey ButtonPressed;
	this->OnSelected(TouchedActor, ButtonPressed);
}

void AOpenSettingButton::SetButtonRespondable(bool bIsRespondable, UWorld* World) 
{
	for (TObjectIterator<AOpenSettingButton> Itr; Itr; ++Itr)
	{
		if (Itr->GetWorld() != World)
		{
			continue;
		}
		Itr->bRespondsToClicks = bIsRespondable;
	}
}

void AOpenSettingButton::OnMouseOver(AActor* Target)
{
	if (!(GetWorld()->IsPaused())) {
		UGameplayStatics::PlaySound2D(GetWorld(), OnHovered);
		GetRenderComponent()->SetSprite(ButtonHoveredSprite);
	}
}

void AOpenSettingButton::OnMouseOverFinished(AActor* Target)
{
	if (!(GetWorld()->IsPaused())) {
		UGameplayStatics::PlaySound2D(GetWorld(), OnHovered);
		GetRenderComponent()->SetSprite(ButtonHoveredSprite);
	}
	GetRenderComponent()->SetSprite(ButtonSprite);
}