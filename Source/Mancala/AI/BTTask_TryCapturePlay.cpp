// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TryCapturePlay.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Mancala/MancalaPit.h"

bool UBTTask_TryCapturePlay::IsPitPlayable(const AMancalaPit* OpponentPit)
{
	AMancalaGameModeBase* GameMode = Cast<AMancalaGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	TArray<AMancalaPit*> OrderedPits = GameMode->GetOrderedPitsFrom(OpponentPit, PlayerType::Opponent);
	AMancalaPit* LastPit = OrderedPits.Last();
	AMancalaPit* OppositePlayerPit = GameMode->GetOppositePit(LastPit);
	if (OppositePlayerPit->GetType() != MancalaPitType::Player)
	{
		return false;
	}
	if (LastPit->GetStoneCount() == 0 && OppositePlayerPit->GetStoneCount() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI: Aimed for capture"));
		return true;
	}

	return false;
}
