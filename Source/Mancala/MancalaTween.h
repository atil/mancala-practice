// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MancalaTween.generated.h"

class USplineComponent;
class AMancalaStone;
class AMancalaPit;

UCLASS()
class MANCALA_API AMancalaTween : public AActor
{
	GENERATED_BODY()

	bool IsPlaying;
	float Timer;

	void DelayedPlay();

public:	

	AMancalaTween();

	virtual void Tick(float DeltaTime) override;

	void Play(AMancalaStone* Stone, const AMancalaPit* Source, AMancalaPit* Target, const float Delay, const bool IsCaptureTween);

	AMancalaPit* TargetPit;

	AMancalaStone* TweeningStone;

	bool IsCaptureTween;

protected:

	virtual void BeginPlay() override;

	USplineComponent* SplineComp;

};
