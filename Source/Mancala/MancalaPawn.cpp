// Fill out your copyright notice in the Description page of Project Settings.

#include "MancalaPawn.h"
#include "MancalaGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void AMancalaPawn::SendPitSelection(AMancalaPit* Pit)
{
	AMancalaGameModeBase* GameMode = Cast<AMancalaGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->OnPitClicked(this, Pit);
}

