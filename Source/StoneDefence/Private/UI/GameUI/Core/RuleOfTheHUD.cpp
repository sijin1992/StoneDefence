// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/Core/RuleOfTheHUD.h"
#include "UObject/ConstructorHelpers.h"
#include "UI/GameUI/UMG/UI_MainScreen.h"

ARuleOfTheHUD::ARuleOfTheHUD()
{
	static ConstructorHelpers::FClassFinder<UUI_MainScreen> MainGame_BPClass(TEXT("/Game/UI/Game/MainScreen_BP"));
	MainScreenClass = MainGame_BPClass.Class;
}

void ARuleOfTheHUD::BeginPlay()
{
	MainScreen = CreateWidget<UUI_MainScreen>(GetWorld(), MainScreenClass);
	MainScreen->AddToViewport();
}
