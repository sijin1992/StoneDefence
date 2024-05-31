
#include "Data/Core/GameData.h"

FGameInstanceDatas::FGameInstanceDatas()
{
	Init();
}

void FGameInstanceDatas::Init()
{
	NumberOfMonster = 0;
	GameDifficulty = 0;
	bAllMainTowerDie = false;
	bGameOver = false;
	bCurrentLevelMissionSuccess = false;
	SpawnMonsterStage = 0;
	CurrentLevel = INDEX_NONE;
	TimeInterval = 0.5f;
	CurrentStagesAreMonsters = 0;
	GameCount = 6000;
	MaxGameCount = 0;
	GoldGrowthTime = 1.0f;
	GoldGrowthMaxTime = 1.5f;
	KillMonstersNumber = 0;
	KillMonsterBossNumber = 0;
	TowersDeathNumber = 0;
	MainTowersDeathNumber = 0;
}

int32 FGameInstanceDatas::GetMaxMonstersNumber()
{
	//把每一波的数量加起来
	int32 MaxMonsterNumber = 0;
	for (auto &Temp : PerNumberOfMonsters)
	{
		MaxMonsterNumber += Temp;
	}
	return MaxMonsterNumber;
}

float FGameInstanceDatas::GetPerOfRemMonsters()
{
	int32 MaxMonsterNumber = GetMaxMonstersNumber();
	if (MaxMonsterNumber != 0)
	{
		return (float)NumberOfMonster / (float)MaxMonsterNumber;
	}
	return 0.0f;
}

//bool FGameInstanceDatas::IsValid()
//{
//
//}
