// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CheckTurn.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_CheckTurn::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const bool IsOpponentTurn = OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("IsOpponentTurn"));

	return IsOpponentTurn ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
