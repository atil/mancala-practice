// Copyright Epic Games, Inc. All Rights Reserved.

#include "MancalaGameModeBase.h"
#include "MancalaPawn.h"
#include "MancalaPit.h"
#include "UI/MancalaHUD.h"
#include "MancalaAIController.h"
#include "MancalaTween.h"
#include "MancalaStone.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

#pragma optimize("", off)
void AMancalaGameModeBase::BeginPlay()
{
	PlayerPawn = Cast<AMancalaPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), PlayerPawnType));
	OpponentPawn = Cast<AMancalaPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), OpponentPawnType));
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("PC is null"));
		return;
	}
	AHUD* Hud = PlayerController->GetHUD();
	HUD = Cast<AMancalaHUD>(Hud);

	TArray<AActor*> PitActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMancalaPit::StaticClass(), PitActors);

	for (AActor* Actor : PitActors)
	{
		
		if (Actor == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Actor is null"));
			continue;
		}
		AMancalaPit* Pit = static_cast<AMancalaPit*>(Actor);
		AllPits.Add(Pit);

		if (Pit->GetType() == MancalaPitType::PlayerGoal)
		{
			PlayerGoal = Pit;
		}
		if (Pit->GetType() == MancalaPitType::OpponentGoal)
		{
			OpponentGoal = Pit;
		}
	}

	AllPits.Sort([](const AMancalaPit& Pit1, const AMancalaPit& Pit2) -> bool
	{
		if (Pit1.GetType() == Pit2.GetType())
		{
			return Pit1.GetIndex() < Pit2.GetIndex();
		}

		return Pit1.GetType() < Pit2.GetType();
	});

	for (AMancalaPit* Pit : AllPits)
	{
		if (Pit->GetType() == MancalaPitType::Opponent)
		{
			OpponentPits.Add(Pit);
		}
		else if (Pit->GetType() == MancalaPitType::Player)
		{
			PlayerPits.Add(Pit);
		}
	}
}
#pragma optimize("", on)

void AMancalaGameModeBase::InitBoard(const bool IsPlayerTurn)
{
	for (AMancalaPit* Pit : AllPits)
	{
		Pit->InitStones();
	}

	if (IsPlayerTurn)
	{
		CurrentTurn = PlayerType::Player;
		HUD->SetStatusText("You start");
	}
	else
	{
		CurrentTurn = PlayerType::Opponent;
		AMancalaAIController* AIController = Cast<AMancalaAIController>(OpponentPawn->GetController());
		AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsOpponentTurn"), true);
		HUD->SetStatusText("Opponent starts");
	}
}

void AMancalaGameModeBase::OnPitClicked(AMancalaPawn* CurrentPawn, AMancalaPit* ClickedPit)
{
	if (TweeningStoneCount > 0)
	{
		return;
	}

	const bool ClickerIsPlayer = CurrentPawn->PlayerType == PlayerType::Player;

	if (ClickerIsPlayer && ClickedPit->GetType() != MancalaPitType::Player)
	{
		return;
	}
	if (!ClickerIsPlayer && ClickedPit->GetType() != MancalaPitType::Opponent)
	{
		return;
	}

	// This is set to false here no matter what. The actual value is set at the end of turn
	AMancalaAIController* AIController = Cast<AMancalaAIController>(OpponentPawn->GetController());
	AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsOpponentTurn"), false);

	PlayTurn(ClickedPit, CurrentTurn);
}

void AMancalaGameModeBase::PlayTurn(AMancalaPit* ClickedPit, const PlayerType ForPlayer)
{
	TArray<AMancalaPit*> OrderedPits = GetOrderedPitsFrom(ClickedPit, ForPlayer);

	TArray<AMancalaStone*> Stones = ClickedPit->RemoveStones();

	ClickedPit->ResetStoneSlots();

	TweeningStoneCount = Stones.Num();
	for (int i = 0; i < TweeningStoneCount; i++)
	{
		AMancalaTween* Tween = GetWorld()->SpawnActor<AMancalaTween>(TweenType);
		Tween->Play(Stones[i], ClickedPit, OrderedPits[i], (i + 0.1f) * 0.4f, false);
	}

	if (ForPlayer == PlayerType::Opponent)
	{
		HUD->SetStatusText("Opponent playing");
	}
	else
	{
		HUD->SetStatusText("You played");
	}
}

TArray<AMancalaPit*> AMancalaGameModeBase::GetOrderedPitsFrom(const AMancalaPit* ThePit, const PlayerType ForPlayer) const
{
	const int PitCount = ThePit->GetStoneCount();
	TArray<AMancalaPit*> Arr;

	for (int i = 0; i < AllPits.Num(); i++)
	{
		if (AllPits[i] == ThePit) // Start counting
		{
			i++; // Skip self
			for (int j = i, k = 0; k < PitCount; j++, k++)
			{
				j = j % AllPits.Num();
				if (j >= AllPits.Num() || j < 0)
				{
					UE_LOG(LogTemp, Error, TEXT("WAT j: %d == pit count: %d == i: %d == pit name: %s == stone count: %d"), 
						j, AllPits.Num(), i, *(ThePit->GetName()), PitCount);
					continue;
				}
				AMancalaPit* PitToQueue = AllPits[j];
				if (PitToQueue->IsOpponentGoalOf(ForPlayer)) // Skip opponent goal
				{
					k--;
					continue;
				}

				Arr.Add(PitToQueue);
			}

			break;
		}
	}

	return Arr;
}

TArray<AMancalaPit*> AMancalaGameModeBase::GetAvailableOpponentPits() const
{
	TArray<AMancalaPit*> AvailableOpponentPits;

	for (AMancalaPit* Pit : OpponentPits)
	{
		if (Pit->GetStoneCount() > 0)
		{
			AvailableOpponentPits.Add(Pit);
		}
	}

	return AvailableOpponentPits;
}

void AMancalaGameModeBase::OnTweenDone(AMancalaTween* Tween)
{
	Tween->TargetPit->AddStone(Tween->TweeningStone);
	Tween->TargetPit->ResetStoneSlots();

	if (--TweeningStoneCount == 0) // Last stone
	{
		OnTurnEnd(Tween);
	}
}

void AMancalaGameModeBase::OnTurnEnd(AMancalaTween* LastTween)
{
	bool DoSwitchTurn = true;
	if (LastTween->TargetPit->IsGoalPit())
	{
		if (!LastTween->IsCaptureTween)
		{
			HUD->ShowFloatingText("Play again!", LastTween->TargetPit->GetScreenLocation());
		}

		DoSwitchTurn = false;
	}
	else
	{
		AMancalaPit* OppositePit = GetOppositePit(LastTween->TargetPit);

		// Capture check
		if (LastTween->TargetPit->GetStoneCount() == 1 // Only the last added one
			&& LastTween->TargetPit->BelongsTo(CurrentTurn) // Only landing on own pits
			&& OppositePit->GetStoneCount() > 0) // Opposite pit has stuff to capture
		{
			Capture(LastTween, OppositePit);
		}
	}

	PlayerType VictoriousPlayer;
	if (CheckEndCondition(VictoriousPlayer))
	{
		FString Message;
		switch (VictoriousPlayer)
		{
		case PlayerType::Player:
			Message = "You won!";
			break;
		case PlayerType::Opponent:
			Message = "You lost!";
			break;
		case PlayerType::None:
			Message = "Tie!";
			break;
		}

		HUD->SetStatusText("");
		HUD->ShowEnd(Message);

		return;
	}

	if (DoSwitchTurn)
	{
		if (CurrentTurn == PlayerType::Opponent)
		{
			CurrentTurn = PlayerType::Player;
		}
		else
		{
			CurrentTurn = PlayerType::Opponent;
		}
	}

	if (CurrentTurn == PlayerType::Opponent)
	{
		HUD->SetStatusText("Opponent turn");
	}
	else
	{
		HUD->SetStatusText("Your turn");
	}

	AMancalaAIController* AIController = Cast<AMancalaAIController>(OpponentPawn->GetController());
	AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsOpponentTurn"), CurrentTurn == PlayerType::Opponent);
}

void AMancalaGameModeBase::Capture(AMancalaTween* LastTweenOnCapturedPit, AMancalaPit* CapturedPit)
{
	// Captured!
	TArray<AMancalaStone*> Stones = CapturedPit->RemoveStones();
	CapturedPit->ResetStoneSlots();

	AMancalaPit* Goal = CurrentTurn == PlayerType::Player ? PlayerGoal : OpponentGoal;
				
	TweeningStoneCount = Stones.Num();
	for (int i = 0; i < TweeningStoneCount; i++)
	{
		AMancalaTween* CapturedStoneTween = GetWorld()->SpawnActor<AMancalaTween>(TweenType);
		CapturedStoneTween->Play(Stones[i], CapturedPit, Goal, (i + 0.1f) * 0.1f, true);
	}

	TArray<AMancalaStone*> SingleStone = LastTweenOnCapturedPit->TargetPit->RemoveStones();
	LastTweenOnCapturedPit->TargetPit->ResetStoneSlots();
	LastTweenOnCapturedPit = GetWorld()->SpawnActor<AMancalaTween>(TweenType);
	LastTweenOnCapturedPit->Play(SingleStone[0], CapturedPit, Goal, 0.1f * 0.1f, true);

	HUD->ShowFloatingText("Captured", CapturedPit->GetScreenLocation());
}

bool AMancalaGameModeBase::CheckEndCondition(PlayerType& VictoriousPlayer) const
{
	bool AllPlayerPitsEmpty = true;
	for (AMancalaPit* PlayerPit : PlayerPits)
	{
		AllPlayerPitsEmpty &= PlayerPit->GetStoneCount() == 0;
	}

	bool AllOpponentPitsEmpty = true;
	for (AMancalaPit* OpponentPit : OpponentPits)
	{
		AllOpponentPitsEmpty &= OpponentPit->GetStoneCount() == 0;
	}

	if (AllPlayerPitsEmpty || AllOpponentPitsEmpty) // Game over
	{
		const int PlayerStoneCount = PlayerGoal->GetStoneCount();
		const int OpponentStoneCount = OpponentGoal->GetStoneCount();

		if (PlayerStoneCount > OpponentStoneCount)
		{
			VictoriousPlayer = PlayerType::Player;
		}
		else if (PlayerStoneCount < OpponentStoneCount)
		{
			VictoriousPlayer = PlayerType::Opponent;
		}
		else
		{
			VictoriousPlayer = PlayerType::None;
		}

		return true;
	}

	return false;
}

AMancalaPit* AMancalaGameModeBase::GetOppositePit(const AMancalaPit* Pit) const
{
	if (Pit->IsGoalPit())
	{
		UE_LOG(LogTemp, Error, TEXT("Not valid for goal pits"));
		checkNoEntry();
		return nullptr;
	}

	MancalaPitType TargetPitType;
	if (Pit->GetType() == MancalaPitType::Player)
	{
		TargetPitType = MancalaPitType::Opponent;
	}
	else if (Pit->GetType() == MancalaPitType::Opponent)
	{
		TargetPitType = MancalaPitType::Player;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Not valid for goal pits"));
		checkNoEntry();
		return nullptr;
	}

	const int TargetPitIndex = FMath::Abs(5 - Pit->GetIndex());
	for (int i = 0; i < AllPits.Num(); i++)
	{
		AMancalaPit* P = AllPits[i];

		if (P->GetIndex() == TargetPitIndex && P->GetType() == TargetPitType)
		{
			return P;
		}
	}

	checkf(false, TEXT("We should've found something here %s"), *FString::FromInt(TargetPitIndex));
	return nullptr;
}

bool AMancalaGameModeBase::CanBePlayedTo(const AMancalaPit* PitToBeLast, const PlayerType ByPlayer) const
{
	for (AMancalaPit* Pit : AllPits)
	{
		if (!Pit->BelongsTo(ByPlayer) // The pits the player can click to
			|| Pit->GetStoneCount() == 0) // Non-empty pits
		{
			continue;
		}
		TArray<AMancalaPit*> OrderedPits = GetOrderedPitsFrom(Pit, ByPlayer);
		if (OrderedPits.Last() == PitToBeLast)
		{
			return true;
		}
	}

	return false;
}

FString AMancalaGameModeBase::GetTurnName() const
{
	if (CurrentTurn == PlayerType::Opponent)
	{
		return TEXT("Opponent turn");
	}
	else
	{
		return TEXT("Your turn");
	}
}

void AMancalaGameModeBase::CheatEnd()
{
	for (AMancalaPit* OpponentPit : OpponentPits)
	{
		auto RemovedStones = OpponentPit->RemoveStones();
		for (AMancalaStone* Stone : RemovedStones)
		{
			Stone->Destroy();
		}
	}

	// #COPYPASTE
	PlayerType VictoriousPlayer;
	CheckEndCondition(VictoriousPlayer);

	FString Message;
	switch (VictoriousPlayer)
	{
	case PlayerType::Player:
		Message = "You won cheater!";
		break;
	case PlayerType::Opponent:
		Message = "You lost cheater!";
		break;
	case PlayerType::None:
		Message = "Tie cheater!";
		break;
	}

	HUD->SetStatusText("");
	HUD->ShowEnd(Message); 
}

