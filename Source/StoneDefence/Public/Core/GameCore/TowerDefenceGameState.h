// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Data/Core/CharacterData.h"
#include "Data/Save/GameSaveData.h"
#include "TowerDefenceGameState.generated.h"

extern FCharacterData CharacterDataNULL;
extern FBuildingTower BuildingTowerNULL;

class ARuleOfTheCharacter;
class UDataTable;
class AMonsters;
class ATowers;
class UGameSaveData;
class UGameSaveSlotList;

UCLASS()
class STONEDEFENCE_API ATowerDefenceGameState : public AGameState
{
	GENERATED_BODY()

	//塔数据
	UPROPERTY()//一定要加这个，否则很容易被GC回收
	UDataTable* AITowerCharacterData;

	//怪物数据
	UPROPERTY()
	UDataTable* AIMonsterCharacterData;

public:
	ATowerDefenceGameState();

	UFUNCTION(BlueprintCallable, Category = Spawn)
	ATowers* SpawnTower(const int32 CharacterID, int32 CharacterLevel, const FVector& Location, const FRotator& Rotator);
	UFUNCTION(BlueprintCallable, Category = Spawn)
	AMonsters* SpawnMonster(const int32 CharacterID, int32 CharacterLevel, const FVector& Location, const FRotator& Rotator);

	UFUNCTION(BlueprintCallable, Category = SaveData)
	bool SaveGameData(int32 SaveNumber);

	UFUNCTION(BlueprintCallable, Category = SaveData)
	bool ReadGameData(int32 SaveNumber);

	//增
	const FCharacterData& AddCharacterData(const FGuid& ID, const FCharacterData &Data);
	//删
	bool RemoveCharacterData(const FGuid& ID);
	//改
	FCharacterData& GetCharacterData(const FGuid& ID);
	//增加塔数据
	const FBuildingTower& AddBuildingTower(const FGuid& ID, const FBuildingTower& Data);
	//获取塔的数据
	FBuildingTower& GetBuildingTower(const FGuid& ID);
	//获取所有塔的ID
	const TArray<const FGuid*> GetBuildingTowerIDs();
	//获取塔、怪物数据表
	bool GetTowerDataFromTable(TArray<const FCharacterData*>& Datas);
	bool GetMonsterDataFromTable(TArray<const FCharacterData*>& Datas);
	const FCharacterData& GetCharacterDataByID(int32 ID, ECharacterType Type = ECharacterType::TOWER);
	//建造列表单元格数据交换
	void RequestInventorySlotSwap(const FGuid& A, const FGuid& B);

protected:
	virtual void BeginPlay() override;
	//获取游戏数据
	UGameSaveData* GetSaveData();
	//获取存档数据
	UGameSaveSlotList* GetGameSaveSlotList();

	//生成角色
	ARuleOfTheCharacter* SpawnCharacter(const int32 CharacterID, int32 CharacterLevel, const UDataTable* InCharacterData, const FVector& Location, const FRotator& Rotator);
	//生成角色的模板函数
	template<class T>
	T* SpawnCharacter(const int32 CharacterID, int32 CharacterLevel, const UDataTable* InCharacterData, const FVector& Location, const FRotator& Rotator)
	{
		return Cast<T>(SpawnCharacter(CharacterID, CharacterLevel, InCharacterData, Location, Rotator));
	}

private:
	//游戏数据
	UPROPERTY()
	UGameSaveData* SaveData;
	//存档数据
	UPROPERTY()
	UGameSaveSlotList* SlotList;
	//缓存数据
	TArray<FCharacterData*> CacheTowerDatas;
	TArray<FCharacterData*> CacheMonsterDatas;
};
