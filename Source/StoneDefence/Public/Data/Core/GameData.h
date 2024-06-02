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
	//获取所有阶段一共需要生成的剩余怪物数
	int32 GetSurplusMonsters();
	
	//获取怪物数量百分比
	float GetPerOfRemMonsters();

	//重新设置SpawnTime
	void ResetSpawnMonsterTime();

	//每次生成怪物时进行阶段计算
	void StageDecision();

	//分配怪物数量梯度，由少到多
	void AssignedMonsterAmount();

	//当前生成怪物的时间是否大于生成怪物的时间间隔
	FORCEINLINE bool IsAlloSpawnMonster() { return CurrentSpawnMonsterTime > TimeInterval; }

	//SaveGame表示不需要保存的变量，Transient不需要保存的变量
	
	//当前关卡
	UPROPERTY(SaveGame)
	int32 CurrentLevel;

	//游戏难度有关
	UPROPERTY(SaveGame)
	int8 GameDifficulty;

	//如果所有的主塔死亡,该值为true,并且禁止建造任何建筑
	UPROPERTY(SaveGame)
	uint8 bAllMainTowerDie : 1;

	//所有塔死亡，游戏结束
	UPROPERTY(Transient)
	uint8 bGameOver : 1;

	//所有塔死亡或者所有怪物死亡，则游戏结束
	UPROPERTY(Transient)
	uint8 bCurrentLevelMissionSuccess : 1;

	//一共有多少个阶段
	UPROPERTY(SaveGame)
	int32 MaxStagesAreMonsters;

	//每个阶段要生成的怪物数量,每生成一个会减少
	UPROPERTY(SaveGame)
	TArray<int32> PerNumberOfMonsters;

	//该关卡要生成的怪物总数
	UPROPERTY(SaveGame)
	int32 MaxMonsters;

	//时间间隔(多久生成一次怪物)
	UPROPERTY(SaveGame)
	float TimeInterval;

	//生成怪物的时间记录
	UPROPERTY(SaveGame)
	float CurrentSpawnMonsterTime;

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