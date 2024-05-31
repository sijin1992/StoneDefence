// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameCore/TowerDefencePlayerController.h"
#include "Core/GameCore/TowerDefenceGameCamrea.h"
#include "UI/Core/UI_Datas.h"

ATowerDefencePlayerController::ATowerDefencePlayerController()
{
	bShowMouseCursor = true;//显示鼠标
	bEnableClickEvents = true;//打开敲击事件
}

void ATowerDefencePlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	float ScreenMoveSpeed = 20.f;
	ScreenMoveUnits.ListenScreenMove(this, ScreenMoveSpeed);

	if (TowerDoll)
	{
		//不让MouseTraceHit影响生成的塔,MouseTraceHit是为以后做射线检测用的
		if (MouseTraceHit.Location != FVector::ZeroVector)
		{
			MouseTraceHit = FHitResult();
		}

		FHitResult TraceOutHit;
		GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel4, true, TraceOutHit);//通过通道检测地面是不是符合建造位置
		TowerDoll->SetActorLocation(TraceOutHit.Location);//设置塔的位置
	}
	else
	{
		GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel5, true, MouseTraceHit);
	}
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

const FHitResult& ATowerDefencePlayerController::GetHitResult()
{
	return MouseTraceHit;
}
