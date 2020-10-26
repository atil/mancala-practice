// Fill out your copyright notice in the Description page of Project Settings.


#include "MancalaTween.h"
#include "MancalaStone.h"
#include "MancalaPit.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"

AMancalaTween::AMancalaTween()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMancalaTween::BeginPlay()
{
	Super::BeginPlay();
	SplineComp = Cast<USplineComponent>(FindComponentByClass(USplineComponent::StaticClass()));

	if (!SplineComp)
	{
		UE_LOG(LogTemp, Error, TEXT("Spline doesn't exist for tween: %s"), *(GetName()));
		return;
	}

	if (SplineComp->GetNumberOfSplinePoints() != 3)
	{
		UE_LOG(LogTemp, Error, TEXT("Spline doesn't have three points: %s"), *(SplineComp->GetName()));
	}
}

void AMancalaTween::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsPlaying)
	{
		return;
	}

	FVector Loc = SplineComp->GetWorldLocationAtTime(Timer);
	TweeningStone->SetActorLocation(Loc);

	Timer += DeltaTime;

	if (Timer > SplineComp->Duration)
	{
		AMancalaGameModeBase* GameMode = Cast<AMancalaGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		GameMode->OnTweenDone(this);
		Destroy();
	}
}

void AMancalaTween::Play(AMancalaStone* Stone, const AMancalaPit* Source, AMancalaPit* Target, const float Delay, const bool CaptureTween)
{
	TweeningStone = Stone;
	TargetPit = Target;
	IsCaptureTween = CaptureTween;

	const FVector SourceLocation = Source->GetSlotLocation();
	const FVector TargetLocation = Target->GetSlotLocation();
	FVector MidLocation = (SourceLocation + TargetLocation) / 2.0f;

	const float Dist = FVector::Dist(SourceLocation, TargetLocation);
	const float BaseHeight = 50;
	// Sqrt to have less difference between small and large distances
	MidLocation.Z += BaseHeight + (FMath::Sqrt(Dist * 8) / 100) * 200;

	SplineComp->Duration += (Dist / 100.0f) * 0.1f;

	SplineComp->SetLocationAtSplinePoint(0, SourceLocation, ESplineCoordinateSpace::World);
	SplineComp->SetLocationAtSplinePoint(1, MidLocation , ESplineCoordinateSpace::World);
	SplineComp->SetLocationAtSplinePoint(2, TargetLocation, ESplineCoordinateSpace::World);
	SplineComp->UpdateSpline();

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMancalaTween::DelayedPlay, Delay, false);
}

void AMancalaTween::DelayedPlay()
{
	Timer = 0.0f;
	IsPlaying = true;
}

