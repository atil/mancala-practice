// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_RandomPlay.h"

bool UBTTask_RandomPlay::IsPitPlayable(const AMancalaPit* OpponentPit)
{
	UE_LOG(LogTemp, Warning, TEXT("AI: Playing random"));
	return FMath::RandBool();
}
