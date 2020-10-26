// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TryProtectPlay.h"
#include "Kismet/GameplayStatics.h"
#include "Mancala/MancalaGameModeBase.h"
#include "Mancala/MancalaPit.h"

bool UBTTask_TryProtectPlay::IsPitPlayable(const AMancalaPit* OpponentPit)
{
	AMancalaGameModeBase* GameMode = Cast<AMancalaGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	// Eliminate the possibility of player capturing "OpponentPit"
	AMancalaPit* OppositeOfLastPit = GameMode->GetOppositePit(OpponentPit);

	if (OppositeOfLastPit->GetType() == MancalaPitType::Player
		&& OppositeOfLastPit->GetStoneCount() == 0
		&& OpponentPit->GetStoneCount() > 0
		&& GameMode->CanBePlayedTo(OppositeOfLastPit, PlayerType::Player))
	{
		UE_LOG(LogTemp, Warning, TEXT("AI: Aiming to protect own"));
		return true;
	}

	return false;
}
