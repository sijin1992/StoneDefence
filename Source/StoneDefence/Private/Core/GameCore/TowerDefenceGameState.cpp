// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameCore/TowerDefenceGameState.h"
#include "../StoneDefenceMacro.h"
#include "Character/Core/RuleOfTheCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Data/Save/GameSaveData.h"
#include "Data/Save/GameSaveSlotList.h"
//关闭优化optimize
#if PLATFORM_WINDOWS
#pragma optimize("",off)
#endif

FCharacterData CharacterDataNULL;
FBuildingTower BuildingTowerNULL;

ATowerDefenceGameState::ATowerDefenceGameState()
{
	PrimaryActorTick.bCanEverTick = true;
	//读取DataTable
	static ConstructorHelpers::FObjectFinder<UDataTable> MyTable_Towers(TEXT("/Game/GameData/TowerData_DT"));
	static ConstructorHelpers::FObjectFinder<UDataTable> MyTable_Monsters(TEXT("/Game/GameData/MonsterData_DT"));

	AITowerCharacterData = MyTable_Towers.Object;
	AIMonsterCharacterData = MyTable_Monsters.Object;

	for (int32 i = 0; i < 21; i++)
	{
		GetSaveData()->BuildingTowers.Add(FGuid::NewGuid(), FBuildingTower());
	}
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
					//RuleOfTheCharacter->GUID = FGuid::NewGuid();
					CharacterData->UpdateHealth();
					AddCharacterData(RuleOfTheCharacter->GUID, *CharacterData);
					//AddCharacterData(RuleOfTheCharacter->GetUniqueID(), *CharacterData);
				}
			}
		}
	}
	return nullptr;
}

const FCharacterData& ATowerDefenceGameState::AddCharacterData(const FGuid& ID, const FCharacterData &Data)
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

const FBuildingTower& ATowerDefenceGameState::AddBuildingTower(const FGuid& ID, const FBuildingTower& Data)
{
	return GetSaveData()->BuildingTowers.Add(ID, Data);
}

FBuildingTower& ATowerDefenceGameState::GetBuildingTower(const FGuid& ID)
{
	if (GetSaveData()->BuildingTowers.Contains(ID))
	{
		return GetSaveData()->BuildingTowers[ID];
	}

	SD_print(Error, "The current [%i] is invalid", *ID.ToString());
	return BuildingTowerNULL;
}

const TArray<const FGuid*> ATowerDefenceGameState::GetBuildingTowerIDs()
{
	TArray<const FGuid*> TowerIDs;
	for (auto &Temp : GetSaveData()->BuildingTowers)
	{
		TowerIDs.Add(&Temp.Key);
	}
	return TowerIDs;
}

bool ATowerDefenceGameState::GetCharacterDataFromTable(TArray<const FCharacterData*>& Datas)
{
	AITowerCharacterData->GetAllRows(TEXT("Character Data"), Datas);
	return Datas.Num() > 0;
}

void ATowerDefenceGameState::RequestInventorySlotSwap(const FGuid& A, const FGuid& B)
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

//打开优化
#if PLATFORM_WINDOWS
#pragma optimize("",on)
#endif