// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MancalaPit.h"

class AMancalaPlayerController;

AMancalaPit::AMancalaPit()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMancalaPit::BeginPlay()
{
	Super::BeginPlay();
	
	StoneSlot = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("StoneSlot")));
	TextRenderComp = FindComponentByClass<UTextRenderComponent>();
}

void AMancalaPit::InitStones()
{
	TArray<AMancalaStone*> OldStones = RemoveStones();
	for (AMancalaStone* Stone : OldStones)
	{
		Stone->Destroy();
	}

	if (!IsGoalPit())
	{
		const int InitialStoneCount = 4;
		for (int i = 0; i < InitialStoneCount; i++)
		{
			AMancalaStone* Stone = GetWorld()->SpawnActor<AMancalaStone>(StoneType);
			Stones.Add(Stone);
		}
	}

	ResetStoneSlots();
}

void AMancalaPit::ResetStoneSlots()
{
	const int StoneCount = Stones.Num();
	const FVector SlotLocation = GetSlotLocation();
	for (int i = 0; i < StoneCount; i++)
	{
		FVector StoneLocation = (FVector::RightVector * StoneSpawnRadius).RotateAngleAxis(i * (360 / StoneCount), FVector::UpVector);
		StoneLocation += SlotLocation;

		Stones[i]->SetActorLocation(StoneLocation);
	}

	TextRenderComp->SetText(FText::AsNumber(StoneCount));
}

TArray<AMancalaStone*> AMancalaPit::RemoveStones()
{
	TArray<AMancalaStone*> Arr(Stones);

	Stones.Empty();

	return Arr;
}

int AMancalaPit::GetStoneCount() const
{
	return Stones.Num();
}

FVector AMancalaPit::GetSlotLocation() const
{
	return StoneSlot->GetComponentLocation();
}

bool AMancalaPit::IsGoalPit() const
{
	return Side == MancalaPitType::OpponentGoal || Side == MancalaPitType::PlayerGoal;
}

void AMancalaPit::AddStone(AMancalaStone* Stone)
{
	Stones.Add(Stone);
}

MancalaPitType AMancalaPit::GetType() const
{
	return Side;
}

int AMancalaPit::GetIndex() const
{
	return Index;
}

bool AMancalaPit::BelongsTo(const PlayerType PlayerType) const
{
	if (PlayerType == PlayerType::Player && GetType() == MancalaPitType::Player)
	{
		return true;
	}
	if (PlayerType == PlayerType::Opponent && GetType() == MancalaPitType::Opponent)
	{
		return true;
	}

	return false;
}

bool AMancalaPit::IsOpponentGoalOf(const PlayerType PlayerType) const
{
	if (PlayerType == PlayerType::Player && GetType() == MancalaPitType::OpponentGoal)
	{
		return true;
	}
	if (PlayerType == PlayerType::Opponent && GetType() == MancalaPitType::PlayerGoal)
	{
		return true;
	}

	return false;
}

FVector2D AMancalaPit::GetScreenLocation() const
{
	const APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	FVector2D ScreenLocation;
	PlayerController->ProjectWorldLocationToScreen(GetSlotLocation(), ScreenLocation);

	return ScreenLocation;
}
