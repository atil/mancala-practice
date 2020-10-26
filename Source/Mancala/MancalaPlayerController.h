// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MancalaPlayerController.generated.h"

class USplineComponent;

enum class PlayerCameraState : int8
{
	Menu,
	Game,
};

UCLASS()
class MANCALA_API AMancalaPlayerController : public APlayerController
{
	GENERATED_BODY()

	PlayerCameraState CameraState;
	
	USplineComponent* StartSpline;

	FVector StartSplineCenter;

	UPROPERTY(EditAnywhere, Category = "Mancala")
	FVector GameCameraPosition;

	float StartSplineMoveTimer;

	void OnClick();

	void CheatEnd();


protected:

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

public:

	void SetCameraState(PlayerCameraState State);

	virtual void Tick(float DeltaTime) override;
	
};
