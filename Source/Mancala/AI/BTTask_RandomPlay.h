// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BTTask_PlayBase.h"
#include "BTTask_RandomPlay.generated.h"

/**
 * 
 */
UCLASS()
class MANCALA_API UBTTask_RandomPlay : public UBTTask_PlayBase
{
	GENERATED_BODY()

protected:	

	virtual bool IsPitPlayable(const AMancalaPit* OpponentPit) override;
};
