// Fill out your copyright notice in the Description page of Project Settings.

#include "MancalaHUD.h"
#include "MancalaAnimatedWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Mancala/MancalaGameModeBase.h"
#include "Mancala/MancalaPlayerController.h"

void AMancalaHUD::BeginPlay()
{
	StatusWidget = CreateWidget(GetWorld(), StatusWidgetType);
	StatusWidget->AddToPlayerScreen();
	StatusText = Cast<UTextBlock>(StatusWidget->WidgetTree->FindWidget(FName(TEXT("Text_Turn"))));
	SetStatusText(TEXT(""));

	StartWidget = CreateWidget(GetWorld(), StartWidgetType);
	StartWidget->AddToPlayerScreen();

	EndWidget = CreateWidget(GetWorld(), EndWidgetType);
	EndWidget->AddToPlayerScreen();
	EndText = Cast<UTextBlock>(EndWidget->WidgetTree->FindWidget(FName(TEXT("Text_EndMessage"))));
	EndWidget->SetVisibility(ESlateVisibility::Hidden);
}

void AMancalaHUD::ShowFloatingText(const FString Message, const FVector2D ScreenPos) const
{
	UMancalaAnimatedWidget* FloatingTextWidget = Cast<UMancalaAnimatedWidget>(CreateWidget(GetWorld(), FloatingTextWidgetType));
	FloatingTextWidget->SetPositionInViewport(ScreenPos);
	FloatingTextWidget->AddToPlayerScreen();

	UTextBlock* Text = Cast<UTextBlock>(FloatingTextWidget->WidgetTree->FindWidget(FName(TEXT("Text_Floating"))));
	Text->SetText(FText::FromString(Message));

	FloatingTextWidget->PlayAnimation(FloatingTextWidget->WidgetAnimation);
}

void AMancalaHUD::SetStatusText(const FString Message) const
{
	if (Message.IsEmpty())
	{
		StatusWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		StatusWidget->SetVisibility(ESlateVisibility::Visible);
	}

	StatusText->SetText(FText::FromString(Message));
}

void AMancalaHUD::ShowEnd(const FString Message) const
{
	EndWidget->SetVisibility(ESlateVisibility::Visible);
	EndText->SetText(FText::FromString(Message));
}

void AMancalaHUD::OnGameStartClicked(const bool IsPlayerStart) const
{
	AMancalaGameModeBase* GameMode = Cast<AMancalaGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->InitBoard(IsPlayerStart);

	AMancalaPlayerController* PlayerController = Cast<AMancalaPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PlayerController->SetCameraState(PlayerCameraState::Game);
	
	StartWidget->RemoveFromParent(); 
}

void AMancalaHUD::OnRestartClicked() const
{
	AMancalaGameModeBase* GameMode = Cast<AMancalaGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->InitBoard(true);
	EndWidget->SetVisibility(ESlateVisibility::Hidden);
}
