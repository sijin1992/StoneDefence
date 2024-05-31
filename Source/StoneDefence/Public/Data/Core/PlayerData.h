#pragma once

#include "CoreMinimal.h"
#include "PlayerData.generated.h"

USTRUCT()
struct FPlayerSkillData
{
	GENERATED_USTRUCT_BODY()

	FPlayerSkillData();

	void Init();

	bool IsValid();

	//技能ID
	UPROPERTY(SaveGame)
	int32 SkillID;
	//技能CD
	UPROPERTY(SaveGame)
	float SkillCD;
	//最大技能CD
	UPROPERTY(SaveGame)
	float MaxSkillID;
	//技能数
	UPROPERTY(SaveGame)
	int32 SkillNumber;

};

USTRUCT()
struct FPlayerData
{
	GENERATED_USTRUCT_BODY()

	FPlayerData();

	void Init();

	bool IsValid();

	//玩家ID
	UPROPERTY(SaveGame)
	int32 PlayID;

	//玩家名字
	UPROPERTY(SaveGame)
	FName PlayName;

	//玩家账户
	UPROPERTY(SaveGame)
	FString Account;

	//游戏金币
	UPROPERTY(SaveGame)
	int32 GameGold;

	//钻石 需要购买
	UPROPERTY(SaveGame)
	int32 Diamonds;

	//铜币
	UPROPERTY(SaveGame)
	int32 Copper;

	//玩家拥有的技能
	UPROPERTY(SaveGame)
	TArray<int32> SkillID;

	//玩家游戏中数据的状态
	UPROPERTY(SaveGame)
	TArray<FPlayerSkillData> SkillData;
};