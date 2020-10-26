// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PlayBase.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Mancala/MancalaGameModeBase.h"
#include "Mancala/MancalaPawn.h"
#include "Mancala/MancalaPit.h"

EBTNodeResult::Type UBTTask_PlayBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMancalaGameModeBase* GameMode = Cast<AMancalaGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	TArray<AMancalaPit*> AvailableOpponentPits = GameMode->GetAvailableOpponentPits();
	for (AMancalaPit* OpponentPit : AvailableOpponentPits)
	{
		if (OpponentPit->GetStoneCount() == 0)
		{
			continue;
		}

		if (IsPitPlayable(OpponentPit))
		{
			AMancalaPawn* OpponentPawn = Cast<AMancalaPawn>(OwnerComp.GetAIOwner()->GetPawn());
			OpponentPawn->SendPitSelection(OpponentPit);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}

bool UBTTask_PlayBase::IsPitPlayable(const AMancalaPit* OpponentPit)
{
	return false;
}
