// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Save/PlayerSaveData.h"
#include "Core/GameCore/TowerDefenceGameState.h"
#include "Data/BuildingTowerData.h"
#include "Data/CharacterData.h"
#include "Core/GameCore/TowerDefencePlayerState.h"
#include "../StoneDefenceUtils.h"
#include "Core/GameCore/TowerDefencePlayerController.h"

void UPlayerSaveData::InitSaveGame(UWorld* InWorld)
{
	if (ATowerDefenceGameState* InGameState = InWorld->GetGameState<ATowerDefenceGameState>())
	{
		const TArray<FCharacterData*>& Datas = InGameState->GetTowerDataFromTable();

		//背包、建造列表数据
		for (int32 i = 0; i < 21; i++)
		{
			FBuildingTower BuildingTower;
			if (Datas.IsValidIndex(i))
			{
				BuildingTower.TowerID = Datas[i]->ID;
				BuildingTower.NeedGold = Datas[i]->Gold;
				BuildingTower.MaxConstructionTowersCD = Datas[i]->CD;
				BuildingTower.Icon = Datas[i]->Icon.LoadSynchronous();
			}
			BuildingTowers.Add(FGuid::NewGuid(), BuildingTower);
		}

		//玩家技能数据
		for (int32 i = 0; i < 10; i++)
		{
			FGuid ID = FGuid::NewGuid();
			if (PlayerData.SkillID.IsValidIndex(i))
			{
				AddPlayerSkill(InWorld , &ID, PlayerData.SkillID[i]);
			}
			else
			{
				PlayerSkillDatas.Add(ID, FPlayerSkillData());
			}
		}
	}
}

void UPlayerSaveData::AddPlayerSkill(UWorld* InWorld, const FGuid* Guid, int32 SkillID)
{
	if (ATowerDefencePlayerState* InPlayerState = InWorld->GetFirstPlayerController()->GetPlayerState<ATowerDefencePlayerState>())
	{
		if (const FPlayerSkillData* SkillData = InPlayerState->GetPlayerSkillDataFromTable(SkillID))
		{
			if (PlayerSkillDatas.Contains(*Guid))
			{
				PlayerSkillDatas[*Guid] = *SkillData;
			}
			else
			{
				PlayerSkillDatas.Add(*Guid, *SkillData);
			}

			//通知客户端添加玩家技能
			StoneDefenceUtils::CallUpdateAllClient(GetWorld(), [&](ATowerDefencePlayerController* MyPlayerController)
				{
					MyPlayerController->UpdatePlayerSkill_Client(*Guid, false);
				});
		}
	}
}
