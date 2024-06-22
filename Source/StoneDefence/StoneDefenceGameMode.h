// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "StoneDefenceGameMode.generated.h"

class ATowers;
class AMonsters;
class ARuleOfTheCharacter;
class UDataTable;
class ATowerDefencePlayerController;
/**
 * 
 */
UCLASS()
class STONEDEFENCE_API AStoneDefenceGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	AStoneDefenceGameMode();

	UFUNCTION(BlueprintCallable, Category = Spawn)
	ATowers* SpawnTower(const int32 CharacterID, int32 CharacterLevel, const FVector& Location, const FRotator& Rotator);
	UFUNCTION(BlueprintCallable, Category = Spawn)
	AMonsters* SpawnMonster(const int32 CharacterID, int32 CharacterLevel, const FVector& Location, const FRotator& Rotator);

protected:
	virtual void BeginPlay() override;

	void InitData();

	void InitDataFromArchives();

	virtual void Tick(float DeltaSeconds) override;
	//生成主塔
	void SpawnMainTowerRule();

	ATowers* SpawnTower(const int32 CharacterID, const FVector& Location, const FRotator& Rotator, const FGuid& InCharacterGuid);
	AMonsters* SpawnMonster(const int32 CharacterID, const FVector& Location, const FRotator& Rotator, const FGuid& InCharacterGuid);

	//生成角色
	ARuleOfTheCharacter* SpawnCharacter(const int32 CharacterID, int32 CharacterLevel, const UDataTable* InCharacterData, const FVector& Location, const FRotator& Rotator, const FGuid& InCharacterGuid = FGuid());
	//生成角色的模板函数
	template<class T>
	T* SpawnCharacter(const int32 CharacterID, int32 CharacterLevel, const UDataTable* InCharacterData, const FVector& Location, const FRotator& Rotator, const FGuid& InCharacterGuid = FGuid())
	{
		return Cast<T>(SpawnCharacter(CharacterID, CharacterLevel, InCharacterData, Location, Rotator, InCharacterGuid));
	}

	//更新怪物数据
	void UpdateMonstersRule(float DeltaSeconds);
	//更新玩家数据
	void UpdatePlayerData(float DeltaSeconds);
	//更新游戏数据
	void UpdateGameData(float DeltaSeconds);
	//更新玩家技能
	void UpdatePlayerSkill(float DeltaSeconds);
	//更新技能系统
	void UpdateSkill(float DeltaSeconds);
	//更新背包
	void UpdateInventory(float DeltaSeconds);
};
