// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PlayBase.generated.h"

class AMancalaPit;
/**
 * 
 */
UCLASS()
class MANCALA_API UBTTask_PlayBase : public UBTTaskNode
{
	GENERATED_BODY()

protected:	

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual bool IsPitPlayable(const AMancalaPit* OpponentPit);
};
