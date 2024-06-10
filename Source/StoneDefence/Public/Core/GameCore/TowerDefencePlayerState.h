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

	//玩家技能数据表
	UPROPERTY()
	UDataTable* PlayerSkillData;

public:
	ATowerDefencePlayerState();

	virtual void BeginPlay() override;

	/////////////////////////////////////保存数据/////////////////////////////////////
	//获取所有需要保存的数据
	UPlayerSaveData* GetSaveData();
	//获取玩家数据
	FPlayerData& GetPlayerData();

	//////////////////////////////////玩家技能////////////////////////////////////////
	//从玩家技能数据表中获取玩家技能列表
	const TArray<FPlayerSkillData*>& GetPlayerSkillDataFromTable();
	const FPlayerSkillData* GetPlayerSkillDataFromTable(const int32& PlayerSkillID);
	//获取玩家技能数据
	FPlayerSkillData* GetPlayerSkillData(const FGuid& SlotID);
	//获取所有玩家技能的ID
	const TArray<const FGuid*> GetPlayerSkillIDs();
	//客户端验证技能
	bool IsVerificationSkill(const FGuid& SlotID);
	//使用玩家技能
	void UsePlayerSkill(const FGuid& SlotID);
	//添加玩家技能到UI
	void AddPlayerSkill(const FGuid& SlotID, int32 SkillID);
	///////////////////////////////炮塔///////////////////////////////////////////
	//获取所有塔的ID
	const TArray<const FGuid*> GetBuildingTowerIDs();
	//获取塔的数据
	FBuildingTower& GetBuildingTower(const FGuid& ID);
	FBuildingTower& GetBuildingTowerNULL();
	//增加塔数据
	const FBuildingTower& AddBuildingTower(const FGuid& ID, const FBuildingTower& Data);
	//建造列表单元格数据交换
	void RequestInventorySlotSwap(const FGuid& A, const FGuid& B);

	UFUNCTION(/*Server*/)
	void TowersPrepareBuildingNumber(const FGuid& InventoryGUID);
	UFUNCTION(/*Server*/)
	void SetTowersDragIconState(const FGuid& InventoryGUID, bool bDragIcon);
	UFUNCTION(/*Server*/)
	void TowersConstructionNumber(const FGuid& InventoryGUID, int32 InValue = INDEX_NONE);

protected:
	//需要保存的玩家数据
	UPROPERTY()
	UPlayerSaveData* SaveData;
private:
	UPROPERTY()
	FBuildingTower BuildingTowerNULL;

	//缓存数据
	TArray<FPlayerSkillData*> CachePlayerSkillDatas;
};
