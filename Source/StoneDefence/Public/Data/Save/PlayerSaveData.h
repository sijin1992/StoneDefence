// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "../PlayerData.h"
#include "../BuildingTowerData.h"
#include "PlayerSaveData.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UPlayerSaveData : public USaveGame
{
	GENERATED_BODY()
public:
	//玩家数据
	UPROPERTY(SaveGame)
	FPlayerData PlayerData;

	//建造列表数据
	UPROPERTY(SaveGame)
	TMap<FGuid, FBuildingTower> BuildingTowers;

};
