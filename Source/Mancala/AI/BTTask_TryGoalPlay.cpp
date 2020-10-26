// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TryGoalPlay.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Mancala/MancalaGameModeBase.h"
#include "Mancala/MancalaPit.h"

bool UBTTask_TryGoalPlay::IsPitPlayable(const AMancalaPit* OpponentPit)
{
	AMancalaGameModeBase* GameMode = Cast<AMancalaGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	TArray<AMancalaPit*> OrderedPits = GameMode->GetOrderedPitsFrom(OpponentPit, PlayerType::Opponent);
	AMancalaPit* LastPit = OrderedPits.Last();

	if (LastPit->GetType() == MancalaPitType::OpponentGoal)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI: Aimed for goal"));
		return true;
	}
	
	return false;
}
