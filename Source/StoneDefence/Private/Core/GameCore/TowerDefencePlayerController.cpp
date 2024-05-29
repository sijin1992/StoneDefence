// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameCore/TowerDefencePlayerController.h"
#include "Core/GameCore/TowerDefenceGameCamrea.h"

ATowerDefencePlayerController::ATowerDefencePlayerController()
{
	bShowMouseCursor = true;
}

void ATowerDefencePlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	float ScreenMoveSpeed = 20.f;
	ScreenMoveUnits.ListenScreenMove(this, ScreenMoveSpeed);
}

void ATowerDefencePlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputModeGameAndUI();
}

void ATowerDefencePlayerController::SetInputModeGameAndUI()
{
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
}

void ATowerDefencePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	//鼠标滚轮
	InputComponent->BindAction("MouseWheelUp", IE_Pressed, this, &ATowerDefencePlayerController::MouseWheelUp);
	InputComponent->BindAction("MouseWheelDown", IE_Pressed, this, &ATowerDefencePlayerController::MouseWheelDown);

	InputComponent->BindAction("MouseMiddle", IE_Pressed, this, &ATowerDefencePlayerController::MouseMiddleButtonPressed);
	InputComponent->BindAction("MouseMiddle", IE_Released, this, &ATowerDefencePlayerController::MouseMiddleButtonReleased);
}

static float WheelValue = 15.f;
void ATowerDefencePlayerController::MouseWheelUp()
{
	ATowerDefenceGameCamrea* ZoomCamera = Cast<ATowerDefenceGameCamrea>(GetPawn());
	if (ZoomCamera)
	{
		ZoomCamera->Zoom(true, WheelValue);
	}
}

void ATowerDefencePlayerController::MouseWheelDown()
{
	ATowerDefenceGameCamrea* ZoomCamera = Cast<ATowerDefenceGameCamrea>(GetPawn());
	if (ZoomCamera)
	{
		ZoomCamera->Zoom(false, WheelValue);
	}
}

void ATowerDefencePlayerController::MouseMiddleButtonPressed()
{
	EventMouseMiddlePressed.ExecuteIfBound();
}

void ATowerDefencePlayerController::MouseMiddleButtonReleased()
{
	EventMouseMiddleReleased.ExecuteIfBound();
}
