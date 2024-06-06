// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Data/Save/PlayerSaveData.h"
#include "Data/BuildingTowerData.h"
#include "TowerDefencePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API ATowerDefencePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ATowerDefencePlayerState();

	//获取所有需要保存的数据
	UPlayerSaveData* GetSaveData();
	//获取玩家数据
	FPlayerData& GetPlayerData();

	//获取所有塔的ID
	const TArray<const FGuid*> GetBuildingTowerIDs();
	//获取塔的数据
	FBuildingTower& GetBuildingTower(const FGuid& ID);
	FBuildingTower& GetBuildingTowerNULL();
	//增加塔数据
	const FBuildingTower& AddBuildingTower(const FGuid& ID, const FBuildingTower& Data);
	//建造列表单元格数据交换
	void RequestInventorySlotSwap(const FGuid& A, const FGuid& B);

protected:
	//需要保存的玩家数据
	UPROPERTY()
	UPlayerSaveData* SaveData;
private:
	UPROPERTY()
	FBuildingTower BuildingTowerNULL;
	
};
