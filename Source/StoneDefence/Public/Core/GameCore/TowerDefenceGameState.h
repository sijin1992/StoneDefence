// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Data/Core/CharacterData.h"
#include "TowerDefenceGameState.generated.h"

FCharacterData CharacterDataNULL;
class ARuleOfTheCharacter;
class UDataTable;
class AMonsters;
class ATowers;
/**
 * 
 */
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

	ATowers* SpawnTower(const int32 CharacterID, int32 CharacterLevel, const FVector& Location, FRotator& Rotator);

	AMonsters* SpawnMonster(const int32 CharacterID, int32 CharacterLevel, const FVector& Location, FRotator& Rotator);

	//增
	const FCharacterData& AddCharacterData(const FGuid &Hash, const FCharacterData &Data);
	//删
	bool RemoveCharacterData(const FGuid &Hash);//Hash即ID
	//改
	FCharacterData& GetCharacterData(const FGuid &Hash);

protected:
	//生成角色
	ARuleOfTheCharacter* SpawnCharacter(const int32 CharacterID, int32 CharacterLevel, const UDataTable* InCharacterData, const FVector& Location, FRotator& Rotator);
	//生成角色的模板函数
	template<class T>
	T* SpawnCharacter(const int32 CharacterID, int32 CharacterLevel, const UDataTable* InCharacterData, const FVector& Location, FRotator& Rotator)
	{
		return Cast<T>(SpawnCharacter(CharacterID, CharacterLevel, InCharacterData, Location, Rotator));
	}

private:
	UPROPERTY()
	TMap<FGuid, FCharacterData> CharacterDatas;
};
