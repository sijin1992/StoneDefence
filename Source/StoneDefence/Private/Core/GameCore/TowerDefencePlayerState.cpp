// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameCore/TowerDefencePlayerState.h"
#include "Data/PlayerData.h"
#include "Data/Save/GameSaveData.h"
#include "Kismet/GameplayStatics.h"
#include "../StoneDefenceMacro.h"

ATowerDefencePlayerState::ATowerDefencePlayerState()
{
	for (int32 i = 0; i < 21; i++)
	{
		GetSaveData()->BuildingTowers.Add(FGuid::NewGuid(), FBuildingTower());
	}
}

UPlayerSaveData* ATowerDefencePlayerState::GetSaveData()
{
	if (!SaveData)
	{
		SaveData = Cast<UPlayerSaveData>(UGameplayStatics::CreateSaveGameObject(UPlayerSaveData::StaticClass()));
	}

	return SaveData;
}

FPlayerData& ATowerDefencePlayerState::GetPlayerData()
{
	return GetSaveData()->PlayerData;
}

const TArray<const FGuid*> ATowerDefencePlayerState::GetBuildingTowerIDs()
{
	TArray<const FGuid*> TowerIDs;
	for (auto& Temp : GetSaveData()->BuildingTowers)
	{
		TowerIDs.Add(&Temp.Key);
	}
	return TowerIDs;
}

FBuildingTower& ATowerDefencePlayerState::GetBuildingTower(const FGuid& ID)
{
	if (GetSaveData()->BuildingTowers.Contains(ID))
	{
		return GetSaveData()->BuildingTowers[ID];
	}

	SD_print(Error, "The current [%i] is invalid", *ID.ToString());
	return BuildingTowerNULL;
}

FBuildingTower& ATowerDefencePlayerState::GetBuildingTowerNULL()
{
	return BuildingTowerNULL;
}

const FBuildingTower& ATowerDefencePlayerState::AddBuildingTower(const FGuid& ID, const FBuildingTower& Data)
{
	return GetSaveData()->BuildingTowers.Add(ID, Data);
}

void ATowerDefencePlayerState::RequestInventorySlotSwap(const FGuid& A, const FGuid& B)
{
	FBuildingTower& ASlot = GetBuildingTower(A);
	FBuildingTower& BSlot = GetBuildingTower(B);

	if (ASlot.IsValid())//如果A里有数据则进行交换
	{
		FBuildingTower TmpSlot = ASlot;
		ASlot = BSlot;
		BSlot = TmpSlot;
	}
	else//否则，直接移动B到A
	{
		ASlot = BSlot;
		BSlot.Init();//将B初始化，也可以达到清空效果
	}
}

void ATowerDefencePlayerState::TowersPrepareBuildingNumber(const FGuid& InventoryGUID)
{
	FBuildingTower& BuildingTower = GetBuildingTower(InventoryGUID);
	if (BuildingTower.IsValid())//服务器验证，防止作弊
	{
		if (BuildingTower.NeedGold <= GetPlayerData().GameGold)
		{
			BuildingTower.TowersPrepareBuildingNumber++;
			GetPlayerData().GameGold -= BuildingTower.NeedGold;

			if (BuildingTower.CurrentConstructionTowersCD <= 0)
			{
				BuildingTower.ResetCD();
			}
		}
	}
}

void ATowerDefencePlayerState::SetTowersDragIconState(const FGuid& InventoryGUID, bool bDragIcon)
{
	FBuildingTower& BuildingTower = GetBuildingTower(InventoryGUID);
	BuildingTower.bDragIcon = bDragIcon;
}

void ATowerDefencePlayerState::TowersConstructionNumber(const FGuid& InventoryGUID, int32 InValue /*= INDEX_NONE*/)
{
	FBuildingTower& BuildingTower = GetBuildingTower(InventoryGUID);
	if (BuildingTower.IsValid())
	{
		BuildingTower.TowersConstructionNumber += InValue;
	}
}
