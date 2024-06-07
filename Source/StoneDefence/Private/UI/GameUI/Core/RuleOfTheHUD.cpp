// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/Core/RuleOfTheHUD.h"
#include "UObject/ConstructorHelpers.h"
#include "UI/GameUI/UMG/UI_MainScreen.h"
#include "UI/GameUI/UMG/Drop/UI_NativeOnDrop.h"

ARuleOfTheHUD::ARuleOfTheHUD()
{
	static ConstructorHelpers::FClassFinder<UUI_MainScreen> MainGame_BPClass(TEXT("/Game/UI/Game/MainScreen_BP"));
	MainScreenClass = MainGame_BPClass.Class;
	static ConstructorHelpers::FClassFinder<UUI_NativeOnDrop> NativeOnDrop_BPClass(TEXT("/Game/UI/Game/Drop/Drop_WBP"));
	NativeOnDropClass = NativeOnDrop_BPClass.Class;
}

void ARuleOfTheHUD::BeginPlay()
{
	MainScreen = CreateWidget<UUI_MainScreen>(GetWorld(), MainScreenClass);
	//MainScreen->AddToViewport();
	MainScreen->AddToPlayerScreen(2);

	NativeOnDropPtr = CreateWidget<UUI_NativeOnDrop>(GetWorld(), NativeOnDropClass);
	NativeOnDropPtr->AddToPlayerScreen(0);
}
