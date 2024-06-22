// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/HallCore/TowerDefenceHallGameMode.h"
#include "UI/HallUI/Core/TowerDefenceHallHUD.h"
#include "Core/HallCore/TowerDefenceHallPawn.h"
#include "Core/GameCore/TowerDefencePlayerController.h"
#include "Core/HallCore/TowerDefenceHallGameState.h"

ATowerDefenceHallGameMode::ATowerDefenceHallGameMode()
{
	HUDClass = ATowerDefenceHallHUD::StaticClass();
	DefaultPawnClass = ATowerDefenceHallPawn::StaticClass();
	PlayerControllerClass = ATowerDefencePlayerController::StaticClass();
	GameStateClass = ATowerDefenceHallGameState::StaticClass();
}

void ATowerDefenceHallGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Hall BeginPlay"));
}
