// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MancalaHUD.generated.h"

class UTextBlock;
class UMancalaAnimatedWidget;

/**
 * 
 */
UCLASS()
class MANCALA_API AMancalaHUD : public AHUD
{
	GENERATED_BODY()

	UUserWidget* StatusWidget;

	UTextBlock* StatusText;

	UUserWidget* StartWidget;

	UUserWidget* EndWidget;

	UTextBlock* EndText;

protected:
	
	virtual void BeginPlay() override;

public:

	//
	// Blueprint interface
	//

	UPROPERTY(EditDefaultsOnly, Category = "Mancala")
	TSubclassOf<UMancalaAnimatedWidget> FloatingTextWidgetType;

	UPROPERTY(EditDefaultsOnly, Category = "Mancala")
	TSubclassOf<UUserWidget> StatusWidgetType;

	UPROPERTY(EditDefaultsOnly, Category = "Mancala")
	TSubclassOf<UUserWidget> StartWidgetType;

	UPROPERTY(EditDefaultsOnly, Category = "Mancala")
	TSubclassOf<UUserWidget> EndWidgetType;

	UFUNCTION(BlueprintCallable, Category = "Mancala")
	void OnGameStartClicked(bool IsPlayerStart) const;

	UFUNCTION(BlueprintCallable, Category = "Mancala")
	void OnRestartClicked() const;

	//
	// Game mode interface
	//

	void ShowFloatingText(const FString Message, const FVector2D ScreenPos) const;

	void SetStatusText(const FString Message) const;

	void ShowEnd(const FString Message) const;
};
