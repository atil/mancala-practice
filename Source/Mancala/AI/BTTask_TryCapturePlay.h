// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTTask_PlayBase.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TryCapturePlay.generated.h"

class AMancalaPit;
/**
 * 
 */
UCLASS()
class MANCALA_API UBTTask_TryCapturePlay : public UBTTask_PlayBase
{
	GENERATED_BODY()

protected:
	
	virtual bool IsPitPlayable(const AMancalaPit* OpponentPit) override;
	
};
