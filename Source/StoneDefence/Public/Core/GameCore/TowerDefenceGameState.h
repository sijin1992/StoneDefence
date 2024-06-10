// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Data/CharacterData.h"
#include "Data/Save/GameSaveData.h"
#include "Data/GameData.h"
#include "../StoneDefenceType.h"
#include "TowerDefenceGameState.generated.h"


class ARuleOfTheCharacter;
class UDataTable;
//class AMonsters;
//class ATowers;
//class UGameSaveData;
class UGameSaveSlotList;
struct FSkillData;

UCLASS()
class STONEDEFENCE_API ATowerDefenceGameState : public AGameState
{
	GENERATED_BODY()

	friend class AStoneDefenceGameMode;

	//塔数据表
	UPROPERTY()//一定要加这个，否则很容易被GC回收
	UDataTable* AITowerCharacterData;

	//怪物数据表
	UPROPERTY()
	UDataTable* AIMonsterCharacterData;

	//角色技能数据表
	UPROPERTY()
	UDataTable* CharacterSkillData;

public:
	ATowerDefenceGameState();

	//获取游戏数据
	FGameInstanceDatas& GetGameData();

	UFUNCTION(BlueprintCallable, Category = SaveData)
	bool SaveGameData(int32 SaveNumber);

	UFUNCTION(BlueprintCallable, Category = SaveData)
	bool ReadGameData(int32 SaveNumber);

	/////////////////////////////////模板角色///////////////////////////////////
	//获取塔、怪物数据表
	const TArray<FCharacterData*>& GetTowerDataFromTable();
	const TArray<FCharacterData*>& GetMonsterDataFromTable();
	/////////////////////////////////角色操作////////////////////////////////////
	//增
	FCharacterData& AddCharacterData(const FGuid& ID, const FCharacterData &Data);
	//删
	bool RemoveCharacterData(const FGuid& ID);
	//改
	FCharacterData& GetCharacterData(const FGuid& ID);
	FCharacterData& GetCharacterDataNULL();

	const FCharacterData& GetCharacterDataByID(int32 ID, ECharacterType Type = ECharacterType::TOWER);
	/////////////////////////////////模板技能/////////////////////////////////////////
	//获取模板技能数据表
	const TArray<FSkillData*>& GetSkillDataFromTable();

	/////////////////////////////////初始化技能数据操作/////////////////////////////////////////
	//将技能数据添加到角色的技能列表中,运行在服务端
	UFUNCTION(/*Server*/)
	void AddSkillDataTemplateToCharacterData(const FGuid& CharacterID, int32 SkillID);
	//验证角色是否已经初始化好技能,运行在客户端
	bool IsVerificationSkillTemplate(const FGuid& CharacterID, int32 SkillID);
	bool IsVerificationSkillTemplate(const FCharacterData& CharacterData, int32 SkillID);

	FSkillData& AddSkillData(const FGuid& CharacterID, const FGuid& SkillID, const FSkillData& Data);
	const FSkillData* GetSkillData(const int32 SkillID);
	FSkillData& GetSkillData(const FGuid& SkillID);
	FSkillData& GetSkillData(const FGuid& CharacterID, const FGuid& SkillID);
	int32 RemoveSkillData(const FGuid& SkillID);
	//设置技能提交类型
	UFUNCTION(/*Server*/)
	bool SetSkillSubmissionType(const FGuid& CharacterID, int32 SkillID, ESubmissionSkillRequestType Type);

	/////////////////////////////////释放挂载技能操作/////////////////////////////////////////
	//判断角色身上是否已经挂上某个技能
	bool IsVerificationSkill(const FGuid& CharacterID, int32 SkillID);
	bool IsVerificationSkill(const FCharacterData& InCharacterData, int32 SkillID);

	//给单个角色挂上技能
	void AddSkill(const FGuid& CharacterGUID, int32& InSkillID);
	void AddSkill(TPair<FGuid, FCharacterData>& InCharacter, FSkillData& InSkill);
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
	TArray<FSkillData*> CacheSkillDatas;
	//空数据
	FCharacterData CharacterDataNULL;
	FSkillData SkillDataNULL;
};
