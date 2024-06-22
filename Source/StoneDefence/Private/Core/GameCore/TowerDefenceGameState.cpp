// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameCore/TowerDefenceGameState.h"
#include "../StoneDefenceMacro.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Data/Save/GameSaveData.h"
#include "Data/Save/GameSaveSlotList.h"
#include "../StoneDefenceUtils.h"
#include "Data/SkillData.h"
#include "Core/GameCore/TowerDefencePlayerController.h"
#include "../StoneDefenceType.h"
#include "Core/GameCore/TowerDefenceGameInstance.h"

//关闭优化optimize
#if PLATFORM_WINDOWS
#pragma optimize("",off)
#endif

ATowerDefenceGameState::ATowerDefenceGameState()
{
	PrimaryActorTick.bCanEverTick = true;
	//读取DataTable
	static ConstructorHelpers::FObjectFinder<UDataTable> MyTable_Towers(TEXT("/Game/GameData/TowerData_DT"));
	static ConstructorHelpers::FObjectFinder<UDataTable> MyTable_Monsters(TEXT("/Game/GameData/MonsterData_DT"));
	static ConstructorHelpers::FObjectFinder<UDataTable> MyTable_Skills(TEXT("/Game/GameData/CharacterSkillData_DT"));

	AITowerCharacterData = MyTable_Towers.Object;
	AIMonsterCharacterData = MyTable_Monsters.Object;
	CharacterSkillData = MyTable_Skills.Object;
}

//////////////////////////////存档数据//////////////////////////////////////////

bool ATowerDefenceGameState::SaveGameData(int32 SaveNumber)
{
	if (SaveData && SlotList)
	{
		SlotList->SlotList.AddGameDataByNumber(SaveNumber);
		return UGameplayStatics::SaveGameToSlot(SaveData, FString::Printf(TEXT("SaveSlot_%i"), SaveNumber), 0)
			&& UGameplayStatics::SaveGameToSlot(SlotList, FString::Printf(TEXT("SlotList")), 0);
	}
	return false;
}

bool ATowerDefenceGameState::ReadGameData(int32 SaveNumber)
{
	SaveData = Cast<UGameSaveData>(UGameplayStatics::LoadGameFromSlot(FString::Printf(TEXT("SaveSlot_%i"), SaveNumber), 0));

	return SaveData != NULL;
}

UGameSaveData* ATowerDefenceGameState::GetSaveData()
{
	if (!SaveData)
	{
		if (UTowerDefenceGameInstance* InGameInstance = GetWorld()->GetGameInstance<UTowerDefenceGameInstance>())
		{
			SaveData = StoneDefenceUtils::GetSave<UGameSaveData>(
				GetWorld(), 
				TEXT("SaveSlot_%i"), 
				InGameInstance->GetCurrentSaveSlotNumber(),
				InGameInstance->GetGameType());
		}
	}

	return SaveData;
}

////////////////////////////////角色//////////////////////////////////////////

const TArray<FCharacterData*>& ATowerDefenceGameState::GetTowerDataFromTable()
{
	if (!CacheTowerDatas.Num())
	{
		AITowerCharacterData->GetAllRows(TEXT("Tower Data"), CacheTowerDatas);
	}
	return CacheTowerDatas;
}

const TArray<FCharacterData*>& ATowerDefenceGameState::GetMonsterDataFromTable()
{
	if (!CacheMonsterDatas.Num())
	{
		AIMonsterCharacterData->GetAllRows(TEXT("Tower Data"), CacheMonsterDatas);
	}
	return CacheMonsterDatas;
}

FCharacterData& ATowerDefenceGameState::AddCharacterData(const FGuid& ID, const FCharacterData &Data)
{
	return GetSaveData()->CharacterDatas.Add(ID, Data);
}

bool ATowerDefenceGameState::RemoveCharacterData(const FGuid& ID)
{
	if (GetSaveData()->CharacterDatas.Remove(ID))
	{
		return true;
	}

	return false;
	/*
	FCharacterData RemoveData;
	for (auto& Tmp : CharacterDatas)
	{
		if (Tmp.GUID == Hash)
		{
			RemoveData = Tmp;
			break;
		}
	}
	for (auto&)
	{
	}
	*/
}

FCharacterData& ATowerDefenceGameState::GetCharacterData(const FGuid& ID)
{
	if (GetSaveData()->CharacterDatas.Contains(ID))
	{
		return GetSaveData()->CharacterDatas[ID];
	}

	SD_print(Error, "The current [%i] is invalid", *ID.ToString());
	return CharacterDataNULL;
	/*
	for (auto &Tmp : CharacterDatas)
	{
		if (Tmp.Key == Hash)
		{
			return Tmp.Value;
		}
	}
	*/
}

const FCharacterData& ATowerDefenceGameState::GetCharacterDataByID(int32 ID, ECharacterType Type /*= ECharacterType::TOWER*/)
{
	auto GetMyCharacterData = [&](const TArray<FCharacterData*> Datas, int32 ID) ->const FCharacterData&
		{
			for (const auto& Temp : Datas)
			{
				if (Temp->ID == ID)
				{
					return *Temp;
				}
			}
			return CharacterDataNULL;
		};

	switch (Type)
	{
	case ECharacterType::TOWER:
	{
		return GetMyCharacterData(GetTowerDataFromTable(), ID);
	}
	case ECharacterType::MONSTER:
	{
		return GetMyCharacterData(GetMonsterDataFromTable(),ID);
	}
	}
	return CharacterDataNULL;
}

FGameInstanceDatas& ATowerDefenceGameState::GetGameData()
{
	return GetSaveData()->GameDatas;
}

FCharacterData& ATowerDefenceGameState::GetCharacterDataNULL()
{
	return CharacterDataNULL;
}

//////////////////////////////技能///////////////////////////////////////

const TArray<FSkillData*>& ATowerDefenceGameState::GetSkillDataFromTable()
{
	if (!CacheSkillDatas.Num())
	{
		CharacterSkillData->GetAllRows(TEXT("Skill Data"), CacheSkillDatas);
	}
	return CacheSkillDatas;
}

void ATowerDefenceGameState::AddSkillDataTemplateToCharacterData(const FGuid& CharacterID, int32 SkillID)
{
	if (const FSkillData* InSkillData = GetSkillData(SkillID))
	{
		for (auto& Temp : GetSaveData()->CharacterDatas)
		{
			if (CharacterID == Temp.Key)
			{
				Temp.Value.CharacterSkills.Add(*InSkillData);
				Temp.Value.CharacterSkills[Temp.Value.CharacterSkills.Num() - 1].ResetCD();
				break;
			}
		}
	}
}

bool ATowerDefenceGameState::IsVerificationSkillTemplate(const FGuid& CharacterID, int32 SkillID)
{
	const FCharacterData& InData = GetCharacterData(CharacterID);
	if (InData.IsValid())
	{
		return IsVerificationSkillTemplate(InData, SkillID);
	}
	return false;
}

bool ATowerDefenceGameState::IsVerificationSkillTemplate(const FCharacterData& CharacterData, int32 SkillID)
{
	for (auto& InSkill : CharacterData.CharacterSkills)
	{
		if (InSkill.ID == SkillID)
		{
			return true;
		}
	}
	return false;
}

FSkillData& ATowerDefenceGameState::AddSkillData(const FGuid& CharacterID, const FGuid& SkillID, const FSkillData& Data)
{
	FCharacterData& InCharacterData = GetCharacterData(CharacterID);
	if (InCharacterData.IsValid())
	{
		return InCharacterData.AdditionalSkillData.Add(SkillID, Data);
	}
	return SkillDataNULL;
}

FSkillData& ATowerDefenceGameState::GetSkillData(const FGuid& SkillID)
{
	for (auto& Temp : GetSaveData()->CharacterDatas)
	{
		if (Temp.Value.AdditionalSkillData.Contains(SkillID))
		{
			return Temp.Value.AdditionalSkillData[SkillID];
		}
	}
	return SkillDataNULL;
}

FSkillData& ATowerDefenceGameState::GetSkillData(const FGuid& CharacterID, const FGuid& SkillID)
{
	FCharacterData& InCharacterData = GetCharacterData(CharacterID);
	if (InCharacterData.IsValid())
	{
		if (InCharacterData.AdditionalSkillData.Contains(SkillID))
		{
			return InCharacterData.AdditionalSkillData[SkillID];
		}
	}
	return SkillDataNULL;
}

const FSkillData* ATowerDefenceGameState::GetSkillData(const int32 SkillID)
{
	const TArray<FSkillData*>& SkillArray = GetSkillDataFromTable();
	for (const auto &Temp : SkillArray)
	{
		if (SkillID == Temp->ID)
		{
			return Temp;
		}
	}
	return nullptr;
}

int32 ATowerDefenceGameState::RemoveSkillData(const FGuid& SkillID)
{
	for (auto& Temp : GetSaveData()->CharacterDatas)
	{
		return Temp.Value.AdditionalSkillData.Remove(SkillID);
	}
	return INDEX_NONE;
}


bool ATowerDefenceGameState::SetSkillSubmissionType(const FGuid& CharacterID, int32 SkillID, ESubmissionSkillRequestType Type)
{
	FCharacterData& InCharacterData = GetCharacterData(CharacterID);
	if (InCharacterData.IsValid())
	{
		for (auto& SkillTemp : InCharacterData.CharacterSkills)
		{
			SkillTemp.SubmissionSkillRequestType = Type;
			return true;
		}
	}
	return false;
}

bool ATowerDefenceGameState::IsVerificationSkill(const FGuid& CharacterID, int32 SkillID)
{
	const FCharacterData& InData = GetCharacterData(CharacterID);
	if (InData.IsValid())
	{
		return IsVerificationSkill(InData, SkillID);
	}
	return false;
}

bool ATowerDefenceGameState::IsVerificationSkill(const FCharacterData& InCharacterData, int32 SkillID)
{
	for (auto& InSkill : InCharacterData.AdditionalSkillData)
	{
		if (InSkill.Value.ID == SkillID)
		{
			return true;
		}
	}
	return false;
}

void ATowerDefenceGameState::AddSkill(TPair<FGuid, FCharacterData>& InCharacter, FSkillData& InSkill)
{
	if (!IsVerificationSkill(InCharacter.Value, InSkill.ID))
	{
		FGuid NewSkillID = FGuid::NewGuid();
		InCharacter.Value.AdditionalSkillData.Add(NewSkillID, InSkill).ResetDuration();
		//通知客户端更新添加UI
		StoneDefenceUtils::CallUpdateAllClient(GetWorld(), [&](ATowerDefencePlayerController* MyPlayerController)
			{
				MyPlayerController->AddSkillSlot_S2C(InCharacter.Key, NewSkillID);
			});
	}
}

void ATowerDefenceGameState::AddSkill(const FGuid& CharacterGUID, int32& InSkillID)
{

}

//打开优化
#if PLATFORM_WINDOWS
#pragma optimize("",on)
#endif