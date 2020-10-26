// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MancalaAIController.generated.h"

/**
 * 
 */
UCLASS()
class MANCALA_API AMancalaAIController : public AAIController
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	class UBehaviorTree* BehaviorTree;

protected:
	virtual void BeginPlay() override;
	
};
