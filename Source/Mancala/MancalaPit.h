// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MancalaGameModeBase.h"
#include "MancalaStone.h"
#include "MancalaPit.generated.h"

UENUM()
enum class MancalaPitType : int8
{
	None = 0,
	Player = 1,
	PlayerGoal = 2,
	Opponent = 3,
	OpponentGoal = 4
};

UCLASS()
class MANCALA_API AMancalaPit : public AActor
{
	GENERATED_BODY()

public:	

	AMancalaPit();

	int GetIndex() const;

	TArray<AMancalaStone*> RemoveStones();

	int GetStoneCount() const;

	FVector GetSlotLocation() const;

	bool IsGoalPit() const;

	MancalaPitType GetType() const;

	void ResetStoneSlots();

	void AddStone(AMancalaStone* Stone);

	bool BelongsTo(const PlayerType PlayerType) const;

	bool IsOpponentGoalOf(const PlayerType PlayerType) const;

	FVector2D GetScreenLocation() const;

	void InitStones();

protected:
	
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = "Mancala")
	int Index;

	UPROPERTY(EditAnywhere, Category = "Mancala")
	TEnumAsByte<MancalaPitType> Side;

	TArray<AMancalaStone*> Stones;

	UPROPERTY(EditDefaultsOnly, Category = "Mancala")
	int StoneSpawnRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Mancala")
	TSubclassOf<AMancalaStone> StoneType;

	class UTextRenderComponent* TextRenderComp;

	class USceneComponent* StoneSlot;

};
