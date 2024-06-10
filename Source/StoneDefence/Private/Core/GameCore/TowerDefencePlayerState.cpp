// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameCore/TowerDefencePlayerState.h"
#include "Data/PlayerData.h"
#include "Data/Save/GameSaveData.h"
#include "Kismet/GameplayStatics.h"
#include "../StoneDefenceMacro.h"
#include "UObject/ConstructorHelpers.h"
#include "../StoneDefenceUtils.h"
#include "Core/GameCore/TowerDefencePlayerController.h"

ATowerDefencePlayerState::ATowerDefencePlayerState()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> MyTable_Skills(TEXT("/Game/GameData/PlayerSkillData_DT"));
	PlayerSkillData = MyTable_Skills.Object;
}

void ATowerDefencePlayerState::BeginPlay()
{
	Super::BeginPlay();
	//背包、建造列表数据
	for (int32 i = 0; i < 21; i++)
	{
		GetSaveData()->BuildingTowers.Add(FGuid::NewGuid(), FBuildingTower());
	}
	//玩家技能数据
	for (int32 i = 0; i < 10; i++)
	{
		GetSaveData()->PlayerSkillDatas.Add(FGuid::NewGuid(), FPlayerSkillData());
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

const TArray<FPlayerSkillData*>& ATowerDefencePlayerState::GetPlayerSkillDataFromTable()
{
	if (!CachePlayerSkillDatas.Num())
	{
		PlayerSkillData->GetAllRows(TEXT("Player Skill Data"), CachePlayerSkillDatas);
	}
	return CachePlayerSkillDatas;
}

const FPlayerSkillData* ATowerDefencePlayerState::GetPlayerSkillDataFromTable(const int32& PlayerSkillID)
{
	const TArray<FPlayerSkillData*>& InSkillDatas = GetPlayerSkillDataFromTable();
	for (auto& Temp : InSkillDatas)
	{
		if (Temp->ID == PlayerSkillID)
		{
			return Temp;
		}
	}
	return nullptr;
}

FPlayerSkillData* ATowerDefencePlayerState::GetPlayerSkillData(const FGuid& SlotID)
{
	if (GetSaveData()->PlayerSkillDatas.Contains(SlotID))
	{
		return &GetSaveData()->PlayerSkillDatas[SlotID];
	}
	return nullptr;
}

const TArray<const FGuid*> ATowerDefencePlayerState::GetPlayerSkillIDs()
{
	TArray<const FGuid*> PlayerSkillIDs;
	for (auto& Temp : GetSaveData()->PlayerSkillDatas)
	{
		PlayerSkillIDs.Add(&Temp.Key);
	}
	return PlayerSkillIDs;
}

bool ATowerDefencePlayerState::IsVerificationSkill(const FGuid& SlotID)
{
	if (FPlayerSkillData* InData = GetPlayerSkillData(SlotID))
	{
		if (InData->IsValid() && InData->SkillNumber > 0 && InData->GetCDPercent() <= 0.0f)
		{
			return true;
		}
	}
	return false;
}

void ATowerDefencePlayerState::UsePlayerSkill(const FGuid& SlotID)
{
	if (FPlayerSkillData* InData = GetPlayerSkillData(SlotID))
	{
		if (InData->IsValid())
		{
			InData->SkillNumber--;
			InData->ResetCD();
			//通知客户端更新玩家技能
			StoneDefenceUtils::CallUpdateAllClient(GetWorld(), [&](ATowerDefencePlayerController* MyPlayerController)
				{
					MyPlayerController->SpawnPlayerSkill_Client(InData->ID);
				});
		}
	}
}

void ATowerDefencePlayerState::AddPlayerSkill(const FGuid& SlotID, int32 SkillID)
{
	if (const FPlayerSkillData* SkillData = GetPlayerSkillDataFromTable(SkillID))
	{
		GetSaveData()->PlayerSkillDatas[SlotID] = *SkillData;

		//通知客户端添加玩家技能
		StoneDefenceUtils::CallUpdateAllClient(GetWorld(), [&](ATowerDefencePlayerController* MyPlayerController)
			{
				MyPlayerController->UpdatePlayerSkill_Client(SlotID, false);
			});
	}
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
