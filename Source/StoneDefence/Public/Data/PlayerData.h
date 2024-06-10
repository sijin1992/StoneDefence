#pragma once

#include "CoreMinimal.h"
#include "../StoneDefenceType.h"
#include "PlayerSkillData.h"
#include "PlayerData.generated.h"

USTRUCT()
struct FPlayerData
{
	GENERATED_USTRUCT_BODY()

	FPlayerData();

	void Init();

	bool IsValid();
	//是否允许增加金币
	bool IsAllowIncrease();

	//玩家ID
	UPROPERTY(SaveGame)
	int32 PlayID;

	//玩家名字
	UPROPERTY(SaveGame)
	FName PlayName;

	//玩家账户
	UPROPERTY(SaveGame)
	FString Account;

	//玩家阵营
	UPROPERTY(SaveGame)
	TEnumAsByte<ETeam> Team;

	//游戏金币
	UPROPERTY(SaveGame)
	int32 GameGold;

	//赠送游戏金币的计时器
	UPROPERTY(SaveGame)
	float GameGoldTime;

	//多长时间赠送一次游戏金币
	UPROPERTY(SaveGame)
	float MaxGameGoldTime;

	//钻石 需要购买
	UPROPERTY(SaveGame)
	int32 Diamonds;

	//铜币
	UPROPERTY(SaveGame)
	int32 Copper;

	//玩家拥有的技能
	UPROPERTY(SaveGame)
	TArray<int32> SkillID;
};