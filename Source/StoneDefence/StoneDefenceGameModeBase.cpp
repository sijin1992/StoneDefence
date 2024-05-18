// Copyright Epic Games, Inc. All Rights Reserved.


#include "StoneDefenceGameModeBase.h"
#include "Core/GameCore/TowerDefenceGameState.h"
#include "Core/GameCore/TowerDefencePlayerController.h"
#include "Core/GameCore/TowerDefenceGameCamrea.h"
#include "Core/GameCore/TowerDefencePlayerState.h"
#include "UI/GameUI/Core/RuleOfTheHUD.h"

AStoneDefenceGameModeBase::AStoneDefenceGameModeBase()
{
	//加载GameState
	GameStateClass = ATowerDefenceGameState::StaticClass();
	//加载Controller
	PlayerControllerClass = ATowerDefencePlayerController::StaticClass();
	//加载ControllerPawn
	DefaultPawnClass = ATowerDefenceGameCamrea::StaticClass();
	//加载PlayerState
	PlayerStateClass = ATowerDefencePlayerState::StaticClass();
	//加载HUD
	HUDClass = ARuleOfTheHUD::StaticClass();
}
