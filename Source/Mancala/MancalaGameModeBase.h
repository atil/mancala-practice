// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MancalaGameModeBase.generated.h"

class AMancalaHUD;
class AMancalaPawn;
class AMancalaPit;
class AMancalaTween;
class AMancalaStone;

UENUM()
enum class PlayerType : int8
{
	None UMETA(DisplayName = "None"),
	Player UMETA(DisplayName = "Player"),
	Opponent UMETA(DisplayName = "Opponent")
};

UCLASS()
class MANCALA_API AMancalaGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Mancala")
	TSubclassOf<AMancalaPawn> PlayerPawnType;

	UPROPERTY(EditAnywhere, Category = "Mancala")
	TSubclassOf<AMancalaPawn> OpponentPawnType;
	
	UPROPERTY(EditAnywhere, Category = "Mancala")
	TSubclassOf<AMancalaTween> TweenType;

	AMancalaHUD* HUD;
		
	AMancalaPawn* PlayerPawn;

	AMancalaPawn* OpponentPawn;

	TArray<AMancalaPit*> PlayerPits;

	TArray<AMancalaPit*> OpponentPits;

	AMancalaPit* PlayerGoal;
	
	AMancalaPit* OpponentGoal;

	PlayerType CurrentTurn;

	int TweeningStoneCount;

	void PlayTurn(AMancalaPit* ClickedPit, PlayerType ForPlayer);

	void Capture(AMancalaTween* LastTweenOnCapturedPit, AMancalaPit* CapturedPit);

	bool CheckEndCondition(PlayerType& VictoriousPlayer) const;

public:

	TArray<AMancalaPit*> AllPits;

	virtual void BeginPlay() override;
	
	void OnPitClicked(AMancalaPawn* CurrentPawn, AMancalaPit* ClickedPit);

	void OnTweenDone(AMancalaTween* Tween);

	TArray<AMancalaPit*> GetOrderedPitsFrom(const AMancalaPit* ThePit, const PlayerType ForPlayer) const;

	TArray<AMancalaPit*> GetAvailableOpponentPits() const;

	void OnTurnEnd(AMancalaTween* LastTween);

	AMancalaPit* GetOppositePit(const AMancalaPit* Pit) const;

	bool CanBePlayedTo(const AMancalaPit* PitToBeLast, const PlayerType ByPlayer) const;

	void CheatEnd();

	//
	// Blueprint interface
	//

	UFUNCTION(BlueprintCallable, Category = "Mancala")
	void InitBoard(bool IsPlayerTurn);

	UFUNCTION(BlueprintCallable, Category = "Mancala")
	FString GetTurnName() const;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Mancala")
	void OnEnd(const FString& Message);
};
