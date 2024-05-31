#pragma once

#include "CoreMinimal.h"
#include "GameData.generated.h"

USTRUCT()
struct FGameInstanceDatas
{
	GENERATED_USTRUCT_BODY()
	FGameInstanceDatas();

	void Init();

	//bool IsValid();
	
	//获取当前最大怪物数量
	int32 GetMaxMonstersNumber();

	//获取怪物数量百分比
	float GetPerOfRemMonsters();

	//SaveGame表示不需要保存的变量，Transient不需要保存的变量
	//该关卡内怪物数量
	UPROPERTY(SaveGame)
	int32 NumberOfMonster;
	//游戏难度有关
	UPROPERTY(SaveGame)
	int8 GameDifficulty;

	//如果所有的主塔死亡,该值为true,并且禁止建造任何建筑
	UPROPERTY(SaveGame)
	uint8 bAllMainTowerDie : 1;

	//所有塔死亡，游戏结束
	UPROPERTY(Transient)
	uint8 bGameOver : 1;

	//所有塔死亡，则游戏结束
	UPROPERTY(Transient)
	uint8 bCurrentLevelMissionSuccess : 1;

	//生成怪物阶段
	UPROPERTY(SaveGame)
	int32 SpawnMonsterStage;

	//当前关卡
	UPROPERTY(SaveGame)
	int32 CurrentLevel;

	//时间间隔(多久生成一次怪物)
	UPROPERTY(SaveGame)
	float TimeInterval;

	//当前是哪一波怪物
	int32 CurrentStagesAreMonsters;

	//每波怪物当前数量
	UPROPERTY(SaveGame)
	TArray<int32> PerNumberOfMonsters;

	//总时间的倒计时
	UPROPERTY(SaveGame)
	float GameCount;

	//最大总时间的倒计时
	UPROPERTY(SaveGame)
	float MaxGameCount;

	//黄金产出的时间
	UPROPERTY(Transient)
	float GoldGrowthTime;

	//最大黄金产出的时间
	UPROPERTY(Transient)
	float GoldGrowthMaxTime;

	//杀死敌人总数量
	UPROPERTY(SaveGame)
	int32 KillMonstersNumber;

	//杀死Boss数量
	UPROPERTY(SaveGame)
	int32 KillMonsterBossNumber;

	//塔死亡总数
	UPROPERTY(SaveGame)
	int32 TowersDeathNumber;

	//主塔死亡数
	UPROPERTY(SaveGame)
	int32 MainTowersDeathNumber;
};