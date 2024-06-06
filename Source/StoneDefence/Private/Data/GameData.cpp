
#include "Data/GameData.h"

FGameInstanceDatas::FGameInstanceDatas()
{
	Init();
}

void FGameInstanceDatas::Init()
{
	CurrentLevel = INDEX_NONE;
	GameDifficulty = 0;
	bAllMainTowerDie = false;
	bGameOver = false;
	bCurrentLevelMissionSuccess = false;
	MaxStagesAreMonsters = 4;
	MaxMonsters = 100;
	TimeInterval = 4.0f;
	CurrentSpawnMonsterTime = 0.0f;
	GameCount = 6000;
	MaxGameCount = 0;
	GoldGrowthTime = 1.0f;
	GoldGrowthMaxTime = 1.5f;
	KillMonstersNumber = 0;
	KillMonsterBossNumber = 0;
	TowersDeathNumber = 0;
	MainTowersDeathNumber = 0;
}

int32 FGameInstanceDatas::GetSurplusMonsters()
{
	int32 InSurplus = 0;
	for (auto &Temp : PerNumberOfMonsters)
	{
		InSurplus += Temp;
	}
	return InSurplus;
}

float FGameInstanceDatas::GetPerOfRemMonsters()
{
	if (MaxMonsters != 0)
	{
		return (float)GetSurplusMonsters() / (float)MaxMonsters;
	}
	return 0.0f;
}

void FGameInstanceDatas::ResetSpawnMonsterTime()
{
	CurrentSpawnMonsterTime = 0.0f;
}

void FGameInstanceDatas::StageDecision()
{
	int32 CurrentStagesAreMonsters = PerNumberOfMonsters.Num() - 1; //剩余阶段数
	if (PerNumberOfMonsters.Num())//怪物总波数不为0
	{
		if (PerNumberOfMonsters[CurrentStagesAreMonsters] > 0)//如果当前阶段的怪物要生成数大于0
		{
			PerNumberOfMonsters[CurrentStagesAreMonsters]--;//当前阶段要生成怪物数减一
		}
		else
		{
			//如果当前阶段要生成的怪物数为0,即没有要生成的怪物了，将阶段数减一
			PerNumberOfMonsters.Remove(CurrentStagesAreMonsters);
		}
	}
	else
	{
		bCurrentLevelMissionSuccess = true;
	}
}

void FGameInstanceDatas::AssignedMonsterAmount()
{
	//剩余怪物总数
	int32 CurrentMonsterNumber = MaxMonsters;
	//剩余阶段数
	int32 CurrentStagesNumber = MaxStagesAreMonsters;
	//每阶段的数量
	int32 CurrentAssignedNum = 0;

	if (CurrentMonsterNumber > 1)
	{
		for (int32 i = 0; i < MaxStagesAreMonsters; i++)//每阶段
		{
			//每阶段的平均值
			float StagesNumber = (float)CurrentMonsterNumber / (float)CurrentStagesNumber;

			CurrentStagesNumber--;
			if (CurrentStagesNumber > 0)//不是最后一波
			{
				//随机阶段，从平均值一半到平均值分配
				CurrentAssignedNum = FMath::RandRange(StagesNumber / 2, StagesNumber);
			}
			else
			{
				CurrentAssignedNum = CurrentMonsterNumber;//最后一波
			}

			PerNumberOfMonsters.Add(CurrentAssignedNum);

			CurrentMonsterNumber -= CurrentAssignedNum;
		}
	}
	else
	{
		PerNumberOfMonsters.Add(CurrentMonsterNumber);
	}
}

//bool FGameInstanceDatas::IsValid()
//{
//
//}
