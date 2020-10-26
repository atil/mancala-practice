// Fill out your copyright notice in the Description page of Project Settings.


#include "MancalaPlayerController.h"
#include "MancalaPit.h"
#include "MancalaPawn.h"
#include "EngineUtils.h"
#include "Components/SplineComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MancalaGameModeBase.h"

void AMancalaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;
	CameraState = PlayerCameraState::Menu;

	for (TActorIterator<AActor> ActorItr = TActorIterator<AActor>(GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr->GetName() == TEXT("StartCameraMoveSpline"))
		{
			StartSpline = Cast<USplineComponent>(ActorItr->GetComponentByClass(USplineComponent::StaticClass()));
			StartSplineCenter = ActorItr->GetActorLocation();
			break;
		}
	}

	GameCameraPosition = GetPawn()->GetActorLocation();
}

void AMancalaPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Click", IE_Pressed, this, &AMancalaPlayerController::OnClick); 
	InputComponent->BindAction("CheatEnd", IE_Released, this, &AMancalaPlayerController::CheatEnd);
}

void AMancalaPlayerController::SetCameraState(PlayerCameraState State)
{
	CameraState = State;

	if (CameraState == PlayerCameraState::Game)
	{
		GetPawn()->SetActorLocation(GameCameraPosition);

		const FRotator Rot = UKismetMathLibrary::FindLookAtRotation(GameCameraPosition, StartSplineCenter);
		SetControlRotation(Rot);
	}
}

void AMancalaPlayerController::Tick(float DeltaTime)
{
	if (CameraState == PlayerCameraState::Menu)
	{
		const FVector Loc = StartSpline->GetLocationAtTime(StartSplineMoveTimer, ESplineCoordinateSpace::World);
		GetPawn()->SetActorLocation(Loc);

		const FRotator Rot = UKismetMathLibrary::FindLookAtRotation(Loc, StartSplineCenter);
		SetControlRotation(Rot);

		StartSplineMoveTimer += DeltaTime;

		if (StartSplineMoveTimer > StartSpline->Duration)
		{
			StartSplineMoveTimer = 0;
		}
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst : Bound to input
void AMancalaPlayerController::OnClick()
{
	FVector MouseRayOrigin;
	FVector MouseRayDirection;
	DeprojectMousePositionToWorld(MouseRayOrigin, MouseRayDirection);

	FHitResult HitResult;
	const FVector Start = MouseRayOrigin;
	const FVector End = MouseRayOrigin + MouseRayDirection * 9999.0f;
	const FCollisionObjectQueryParams ObjectQueryParams(ECC_WorldStatic);

	if (!GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, ObjectQueryParams))
	{
		return;
	}

	AMancalaPit* HitPit = Cast<AMancalaPit>(HitResult.GetActor());

	if (HitPit)
	{
		Cast<AMancalaPawn>(GetPawn())->SendPitSelection(HitPit);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst : Bound to input
void AMancalaPlayerController::CheatEnd()
{
	AMancalaGameModeBase* GameMode = Cast<AMancalaGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->CheatEnd();
}

