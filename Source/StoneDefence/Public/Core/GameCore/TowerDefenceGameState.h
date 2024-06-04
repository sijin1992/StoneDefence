// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Data/Core/CharacterData.h"
#include "Data/Save/GameSaveData.h"
#include "Data/Core/GameData.h"
#include "../StoneDefenceType.h"
#include "TowerDefenceGameState.generated.h"


class ARuleOfTheCharacter;
class UDataTable;
//class AMonsters;
//class ATowers;
//class UGameSaveData;
class UGameSaveSlotList;

UCLASS()
class STONEDEFENCE_API ATowerDefenceGameState : public AGameState
{
	GENERATED_BODY()

	friend class AStoneDefenceGameMode;

	//塔数据
	UPROPERTY()//一定要加这个，否则很容易被GC回收
	UDataTable* AITowerCharacterData;

	//怪物数据
	UPROPERTY()
	UDataTable* AIMonsterCharacterData;

public:
	ATowerDefenceGameState();

	//获取游戏数据
	FGameInstanceDatas& GetGameData();

	UFUNCTION(BlueprintCallable, Category = SaveData)
	bool SaveGameData(int32 SaveNumber);

	UFUNCTION(BlueprintCallable, Category = SaveData)
	bool ReadGameData(int32 SaveNumber);

	//获取塔、怪物数据表
	bool GetTowerDataFromTable(TArray<const FCharacterData*>& Datas);
	bool GetMonsterDataFromTable(TArray<const FCharacterData*>& Datas);

	//增
	FCharacterData& AddCharacterData(const FGuid& ID, const FCharacterData &Data);
	//删
	bool RemoveCharacterData(const FGuid& ID);
	//改
	FCharacterData& GetCharacterData(const FGuid& ID);
	FCharacterData& GetCharacterDataNULL();

	const FCharacterData& GetCharacterDataByID(int32 ID, ECharacterType Type = ECharacterType::TOWER);



protected:
	//获取所有需要保存的数据
	UGameSaveData* GetSaveData();
	//获取存档数据
	UGameSaveSlotList* GetGameSaveSlotList();

private:
	//需要保存的游戏数据
	UPROPERTY()
	UGameSaveData* SaveData;
	//存档数据
	UPROPERTY()
	UGameSaveSlotList* SlotList;
	//缓存数据
	TArray<FCharacterData*> CacheTowerDatas;
	TArray<FCharacterData*> CacheMonsterDatas;

	FCharacterData CharacterDataNULL;
};
