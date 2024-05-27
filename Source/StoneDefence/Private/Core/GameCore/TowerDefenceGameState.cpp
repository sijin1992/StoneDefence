// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameCore/TowerDefenceGameState.h"
#include "../StoneDefenceMacro.h"
#include "Character/Core/RuleOfTheCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Data/Save/GameSaveData.h"
#include "Data/Save/GameSaveSlotList.h"

ATowerDefenceGameState::ATowerDefenceGameState()
{
	PrimaryActorTick.bCanEverTick = true;
	//读取DataTable
	static ConstructorHelpers::FObjectFinder<UDataTable> MyTable_Towers(TEXT("/Game/GameData/TowerData_DT"));
	static ConstructorHelpers::FObjectFinder<UDataTable> MyTable_Monsters(TEXT("/Game/GameData/MonsterData_DT"));

	AITowerCharacterData = MyTable_Towers.Object;
	AIMonsterCharacterData = MyTable_Monsters.Object;
}

ATowers* ATowerDefenceGameState::SpawnTower(const int32 CharacterID, int32 CharacterLevel, const FVector& Location, const FRotator& Rotator)
{
	return SpawnCharacter<ATowers>(CharacterID, CharacterLevel, AITowerCharacterData, Location, Rotator);
}

AMonsters* ATowerDefenceGameState::SpawnMonster(const int32 CharacterID, int32 CharacterLevel, const FVector& Location, const FRotator& Rotator)
{
	return SpawnCharacter<AMonsters>(CharacterID, CharacterLevel, AIMonsterCharacterData, Location, Rotator);
}

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

void ATowerDefenceGameState::BeginPlay()
{
	Super::BeginPlay();
	//if (1)//判断是通过读取的方式还是储存
	//{
	//	//创建存储数据
	//	SaveData = Cast<UGameSaveData>(UGameplayStatics::CreateSaveGameObject(UGameSaveData::StaticClass()));
	//}
}

UGameSaveData* ATowerDefenceGameState::GetSaveData()
{
	if (!SaveData)
	{
		SaveData = Cast<UGameSaveData>(UGameplayStatics::CreateSaveGameObject(UGameSaveData::StaticClass()));
	}

	return SaveData;
}

UGameSaveSlotList* ATowerDefenceGameState::GetGameSaveSlotList()
{
	if (!SlotList)
	{
		SlotList = Cast<UGameSaveSlotList>(UGameplayStatics::LoadGameFromSlot(FString::Printf(TEXT("SlotList")), 0));
		if (!SlotList)
		{
			SlotList = Cast<UGameSaveSlotList>(UGameplayStatics::CreateSaveGameObject(UGameSaveSlotList::StaticClass()));
		}
	}

	return SlotList;
}

ARuleOfTheCharacter* ATowerDefenceGameState::SpawnCharacter(
	const int32 CharacterID, 
	int32 CharacterLevel, 
	const UDataTable* InCharacterData,
	const FVector& Location, 
	const FRotator& Rotator)
{
	if (InCharacterData)
	{
		TArray<FCharacterData*> Datas;
		InCharacterData->GetAllRows(TEXT("Character Data"), Datas);

		auto GetCharacterData = [&](int32 ID) ->FCharacterData* 
			{
				for (auto &Temp : Datas)
				{
					if (Temp->ID == ID)
					{
						return Temp;
					}
				}
				return nullptr;
			};


		if (FCharacterData* CharacterData = GetCharacterData(CharacterID))
		{
			UClass* NewClass = CharacterData->CharacterBlueprintKey.LoadSynchronous();//LoadSynchronous读取
			if (GetWorld() && NewClass)
			{
				if (ARuleOfTheCharacter* RuleOfTheCharacter = GetWorld()->SpawnActor<ARuleOfTheCharacter>(NewClass, Location, Rotator))
				{
					CharacterData->UpdateHealth();
					AddCharacterData(RuleOfTheCharacter->GetUniqueID(), *CharacterData);
					//RuleOfTheCharacter->GUID = FGuid::NewGuid();
					//AddCharacterData(RuleOfTheCharacter->GUID, *CharacterData);
				}
			}
		}
	}
	return nullptr;
}

const FCharacterData& ATowerDefenceGameState::AddCharacterData(const uint32& ID, const FCharacterData &Data)
{
	return GetSaveData()->CharacterDatas.Add(ID, Data);
}

bool ATowerDefenceGameState::RemoveCharacterData(const uint32& ID)
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

FCharacterData& ATowerDefenceGameState::GetCharacterData(const uint32& ID)
{
	if (GetSaveData()->CharacterDatas.Contains(ID))
	{
		return GetSaveData()->CharacterDatas[ID];
	}

	SD_print_r(Error, "The current [%i] is invalid", ID);
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
