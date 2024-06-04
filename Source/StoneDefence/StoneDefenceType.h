// Copyright Epic Games, Inc. All Rights Reserved.
//自定义游戏类型，通过复制StoneDefence.h文件并重命名得来
#pragma once

#include "CoreMinimal.h"
#include "StoneDefenceType.generated.h"

UENUM()
enum ETeam
{
	RED,
	BLUE,
	MAX
};


UENUM(BlueprintType)
//角色类型
namespace EGameCharacterType
{
	enum Type
	{
		MINI,
		BOSS,
		MAX
	};
}

UENUM(BlueprintType)
enum class EBulletType :uint8
{
	BULLET_DIRECT_LINE,		//无障碍直线攻击,可以把周围的敌人全部贯穿
	BULLET_LINE,			//非跟踪类型，类似手枪子弹
	BULLET_TRACK_LINE,		//跟踪类型
	BULLET_TRACK_BEZIER,	//以贝赛尔曲线跟踪
	BULLET_RANGE_LINE,		//范围伤害，丢手雷
	BULLET_RANGE,			//范围伤害
	BULLET_CHAIN,			//链条类型，持续伤害类型
};

UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	TOWER,
	MONSTER,
};