// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MancalaGameModeBase.h"
#include "GameFramework/Pawn.h"
#include "MancalaPawn.generated.h"

class AMancalaPit;

UCLASS()
class MANCALA_API AMancalaPawn : public APawn
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Mancala")
	PlayerType PlayerType;

	void SendPitSelection(AMancalaPit* Pit);
};
