#pragma once

#include "CoreMinimal.h"
#include "Core/DataCore.h"
#include "../StoneDefenceType.h"
#include "SkillData.generated.h"

class ARuleOfTheBullet;

USTRUCT(BlueprintType)
struct FSkillData : public FDataCore
{
	GENERATED_USTRUCT_BODY()

	typedef FDataCore Super;

	FSkillData();

	virtual void Init() override;

	//技能类型
	UPROPERTY(EditDefaultsOnly, Category = "Skill Attribute")
	FSkillType SkillType;
	//技能Icon
	UPROPERTY(EditDefaultsOnly, Category = "Skill Attribute")
	TAssetPtr<class UTexture2D> Icon;
	//子弹类型
	UPROPERTY(EditDefaultsOnly, Category = "Skill Attribute")
	TSubclassOf<AActor> BulletClass;
	//技能描述
	UPROPERTY(EditDefaultsOnly, Category = "Skill Attribute")
	FText SkillIntroduce;
	//技能提交类型
	UPROPERTY()
	ESubmissionSkillRequestType SubmissionSkillRequestType;

	//加减金币
	UPROPERTY(EditDefaultsOnly, Category = "Skill Profit")
	float Gold;
	//加减生命值
	UPROPERTY(EditDefaultsOnly, Category = "Skill Profit")
	float Health;
	//加减攻击力
	UPROPERTY(EditDefaultsOnly, Category = "Skill Profit")
	float PhysicalAttack;
	//加减攻击速度
	UPROPERTY(EditDefaultsOnly, Category = "Skill Profit")
	float AttackSpeed;
	//加减护甲
	UPROPERTY(EditDefaultsOnly, Category = "Skill Profit")
	float Armor;
	//加减移动速度
	UPROPERTY(EditDefaultsOnly, Category = "Skill Profit")
	float WalkSpeed;

	//技能范围(0代表全场景所有敌人)
	UPROPERTY(EditDefaultsOnly, Category = "Skill Profit")
	float AttackRange;
	//技能效果最大持续时间
	UPROPERTY(EditDefaultsOnly, Category = "Skill Profit")
	float MaxSkillDuration;
	//技能效果持续时间
	UPROPERTY()
	float SkillDuration;
	//每帧进行更新的持续时间
	UPROPERTY()
	float SkillDurationTime;

	//技能释放CD
	UPROPERTY(EditDefaultsOnly, Category = "Skill Profit")
	float CD;
	//技能释放CD计时器
	UPROPERTY()
	float CDTime;
	//技能是否已经释放过了
	bool bBecomeEffective;

	//获取技能释放CD百分比
	float GetCDPercent() const;
	//重置技能释放的CD
	void ResetCD();

	//获取技能效果持续时间百分比
	float GetDurationPercent() const;
	//重置技能效果持续时间
	void ResetDuration();

	friend bool operator == (const FSkillData& A, const FSkillData& B)
	{
		return A.ID == B.ID;
	}
};