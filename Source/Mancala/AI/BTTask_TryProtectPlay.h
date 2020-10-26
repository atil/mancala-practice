// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTTask_PlayBase.h"
#include "BTTask_TryProtectPlay.generated.h"

class AMancalaPit;
/**
 * 
 */
UCLASS()
class MANCALA_API UBTTask_TryProtectPlay : public UBTTask_PlayBase
{
	GENERATED_BODY()
	
protected:

	virtual bool IsPitPlayable(const AMancalaPit* OpponentPit) override;
};
