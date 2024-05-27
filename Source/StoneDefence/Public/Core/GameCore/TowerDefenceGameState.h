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
class UGameSaveData;
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

	UFUNCTION(BlueprintCallable, Category = Spawn)
	ATowers* SpawnTower(const int32 CharacterID, int32 CharacterLevel, const FVector& Location, const FRotator& Rotator);
	UFUNCTION(BlueprintCallable, Category = Spawn)
	AMonsters* SpawnMonster(const int32 CharacterID, int32 CharacterLevel, const FVector& Location, const FRotator& Rotator);

	UFUNCTION(BlueprintCallable, Category = SaveData)
	bool SaveGameData(int32 SaveNumber);

	UFUNCTION(BlueprintCallable, Category = SaveData)
	bool ReadGameData(int32 SaveNumber);

	//增
	const FCharacterData& AddCharacterData(const uint32& ID, const FCharacterData &Data);
	//删
	bool RemoveCharacterData(const uint32& ID);
	//改
	FCharacterData& GetCharacterData(const uint32& ID);

protected:
	virtual void BeginPlay() override;

	UGameSaveData* GetSaveData();

	//生成角色
	ARuleOfTheCharacter* SpawnCharacter(const int32 CharacterID, int32 CharacterLevel, const UDataTable* InCharacterData, const FVector& Location, const FRotator& Rotator);
	//生成角色的模板函数
	template<class T>
	T* SpawnCharacter(const int32 CharacterID, int32 CharacterLevel, const UDataTable* InCharacterData, const FVector& Location, const FRotator& Rotator)
	{
		return Cast<T>(SpawnCharacter(CharacterID, CharacterLevel, InCharacterData, Location, Rotator));
	}

private:
	UPROPERTY()
	UGameSaveData* SaveData;
};
