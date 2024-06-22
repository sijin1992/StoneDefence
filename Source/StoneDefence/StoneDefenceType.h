// Copyright Epic Games, Inc. All Rights Reserved.
//自定义游戏类型，通过复制StoneDefence.h文件并重命名得来
#pragma once

#include "CoreMinimal.h"
#include "StoneDefenceType.generated.h"

enum EGameSaveType
{
	NONE		= 0b0000,//没有存档
	ARCHIVES	= 0b0001,//有存档
};

//技能是自动提交还是手动提交
UENUM(BlueprintType)
enum class ESubmissionSkillRequestType :uint8
{
	AUTO,	//生成权利交给服务器
	MANUAL,	//生成权利交给客户端
};

//技能类型
UENUM(BlueprintType)
enum class ESkillType:uint8
{
	BURST,//爆发,一次造成的伤害
	SECTION,//区间,在一定范围内属性提升，时间到了就还原属性
	ITERATION,//迭代,持续一定时间内进行恢复
};

//技能目标阵营
UENUM(BlueprintType)
enum class ESkillTargetType :uint8
{
	FRIENDLY_FORCES,//友军技能
	ENEMY//敌人技能
};
//技能效果类型
UENUM(BlueprintType)
enum class ESkillEffectType :uint8
{
	ADD,//增益
	SUBTRACT,//减益
};

USTRUCT(BlueprintType)
struct FSkillType
{
	GENERATED_USTRUCT_BODY()

	FSkillType()
		:SkillType(ESkillType::SECTION),
		SkillTargetType(ESkillTargetType::FRIENDLY_FORCES),
		SkillEffectType(ESkillEffectType::ADD)
	{
	}

	UPROPERTY(EditDefaultsOnly, Category = "Skill Type")
	ESkillType SkillType;

	UPROPERTY(EditDefaultsOnly, Category = "Skill Type")
	ESkillTargetType SkillTargetType;

	UPROPERTY(EditDefaultsOnly, Category = "Skill Type")
	ESkillEffectType SkillEffectType;
};



//队伍类型
UENUM()
enum ETeam
{
	RED,
	BLUE,
	MAX
};

//角色类型
UENUM(BlueprintType)
namespace EGameCharacterType
{
	enum Type
	{
		MINI,
		BOSS,
		MAX
	};
}
//子弹类型
UENUM(BlueprintType)
enum class EBulletType :uint8
{
	BULLET_NONE,			//不产生任何效果
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