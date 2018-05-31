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

	UFUNCTION()
		void OnSelected(AActor* Target, FKey ButtonPressed);

	static void SetButtonRespondable(bool bIsRespondable, UWorld* World);
};
