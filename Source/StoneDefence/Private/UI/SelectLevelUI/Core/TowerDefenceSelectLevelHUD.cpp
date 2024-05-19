// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SelectLevelUI/Core/TowerDefenceSelectLevelHUD.h"
#include "UObject/ConstructorHelpers.h"
#include "UI/SelectLevelUI/UMG/UI_MainSelectLevel.h"

ATowerDefenceSelectLevelHUD::ATowerDefenceSelectLevelHUD()
{
	static ConstructorHelpers::FClassFinder<UUI_MainSelectLevel> MainSelect_BPClass(TEXT("/Game/UI/Select/MainSelect_BP"));
	MainSelecLevelClass = MainSelect_BPClass.Class;
}

void ATowerDefenceSelectLevelHUD::BeginPlay()
{
	Super::BeginPlay();

	MainSelectLevel = CreateWidget<UUI_MainSelectLevel>(GetWorld(), MainSelecLevelClass);
	MainSelectLevel->AddToViewport();
}
