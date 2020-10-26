// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MancalaAnimatedWidget.generated.h"

/**
 * 
 */
UCLASS()
class MANCALA_API UMancalaAnimatedWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim), Category = "Mancala")
	UWidgetAnimation* WidgetAnimation;
	
};
