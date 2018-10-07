// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
#include "OpenSettingButton.generated.h"

/**
 * 
 */
UCLASS()
class FLABRIYA_API AOpenSettingButton : public APaperSpriteActor
{
	GENERATED_BODY()

public:

	AOpenSettingButton();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bRespondsToClicks = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPaperSprite * ButtonSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPaperSprite * ButtonHoveredSprite;

	UFUNCTION()
		void OnSelected(AActor* Target, FKey ButtonPressed);

	UFUNCTION()
		void OnSelectedTouch(ETouchIndex::Type FingerIndex, AActor* TouchedActor);

	UFUNCTION()
		void OnMouseOver(AActor* Target);

	UFUNCTION()
		void OnMouseOverFinished(AActor* Target);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundBase* OnClick;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundBase* OnHovered;

	static void SetButtonRespondable(bool bIsRespondable, UWorld* World);
};
