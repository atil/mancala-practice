// Fill out your copyright notice in the Description page of Project Settings.

#include "MancalaAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void AMancalaAIController::BeginPlay()
{
	Super::BeginPlay();
	
	if (!BehaviorTree)
	{
		return;
	}

	RunBehaviorTree(BehaviorTree);

	GetBlackboardComponent()->SetValueAsBool(TEXT("IsPlayerTurn"), true);
}
