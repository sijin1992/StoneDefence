// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SelectLevelCore/TowerDefenceLevelGameMode.h"
#include "UI/SelectLevelUI/Core/TowerDefenceSelectLevelHUD.h"

ATowerDefenceLevelGameMode::ATowerDefenceLevelGameMode()
{
	HUDClass = ATowerDefenceSelectLevelHUD::StaticClass();
}

void ATowerDefenceLevelGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Level BeginPlay"));
}
