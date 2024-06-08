// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/DataCore.h"
#include "SkillData.h"
#include "CharacterData.generated.h"

USTRUCT(BlueprintType)
struct FCharacterData : public FDataCore
{
	GENERATED_BODY()

	typedef FDataCore Super;

public:
	FCharacterData();

	virtual void Init() override;

	//////////////////////////////资源/////////////////////////////////////////
	//角色蓝图类
	UPROPERTY(EditDefaultsOnly, Category = "Character Class")
	TAssetSubclassOf<class ARuleOfTheCharacter> CharacterBlueprintKey;//也可以用TSubclassOf，因为保存的是路径，所以很占内存

	//角色图片
	UPROPERTY(EditDefaultsOnly, Category = "Character Icon")
	TAssetPtr<class UTexture2D> Icon;
	
	//////////////////////////////////基础属性////////////////////////////////////////
	//角色等级
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Attribute")
	float Lv;

	//最大生命值
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
	float MaxHealth;

	//当前生命值
	UPROPERTY()
	float Health;

	//基础攻击
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
	float PhysicalAttack;

	//护甲
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
	float Armor;

	//当前等级的最大经验值
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
	float MaxEmpircalValue;

	//当前经验值
	UPROPERTY()
	float EmpirceValue;

	//CD
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
	float CD;

	//攻击速度
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
	float AttackSpeed;

	//如果是塔，则代表花费多少金币可以建造
	//如果是怪物，代码消灭它可以获取多少金币
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
	float Gold;

	//角色简介
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
	FText Introduction;

	//角色队伍类型
	TEnumAsByte<ETeam> Team;

	//角色位置
	UPROPERTY()
	FVector Location;

	//角色转向
	UPROPERTY()
	FRotator Rotator;

	
	///////////////////////////增益部分///////////////////////////////////////////////
	//添加金币
	UPROPERTY(EditDefaultsOnly, Category = "Character Profit")
	float AddGold;

	//添加生命值
	UPROPERTY(EditDefaultsOnly, Category = "Character Profit")
	float AddHealth;

	//添加攻击力
	UPROPERTY(EditDefaultsOnly, Category = "Character Profit")
	float AddPhysicalAttack;

	//添加护甲
	UPROPERTY(EditDefaultsOnly, Category = "Character Profit")
	float AddArmor;

	//添加经验值
	UPROPERTY(EditDefaultsOnly, Category = "Character Profit")
	float AddEmpiricalValue;

	//添加攻击速度
	UPROPERTY(EditDefaultsOnly, Category = "Character Profit")
	float AddAttackSpeed;

	//自动恢复生命值
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
	float RestoreHealth;

	//////////////////////////////角色的技能////////////////////////////////////////////

	//添加被动生命值
	UPROPERTY(EditDefaultsOnly, Category = "Character Passive")
	float AddPassiveSkillHealth;
	
	//添加持续恢复生命值
	UPROPERTY(EditDefaultsOnly, Category = "Character Passive")
	float AddContinueHealth;

	//添加被动攻击力
	UPROPERTY(EditDefaultsOnly, Category = "Character Passive")
	float AddPassiveSkillPhysicalAttack;

	//添加被动护甲
	UPROPERTY(EditDefaultsOnly, Category = "Character Passive")
	float AddPassiveSkilldArmor;

	//添加被动攻击速度
	UPROPERTY(EditDefaultsOnly, Category = "Character Passive")
	float AddPassiveSkillAttackSpeed;

	//减少被动CD
	UPROPERTY(EditDefaultsOnly, Category = "Character Passive")
	float ReducePassiveSkillCDTime;

	/////////////////////////////函数/////////////////////////////////////////////
	void UpdateHealth();//更新当前生命值

	bool UpdateEXP(float InExp);//升级经验

	float GetEXPPercent() const;//获取经验百分比

	void UpdateLevel();//直接升级

	/////////////////////////////技能/////////////////////////////////////////////
	//角色拥有的实体技能数据
	UPROPERTY()
	TArray<FSkillData> CharacterSkills;

	//动态添加的技能,别的角色添加到当前角色的技能
	UPROPERTY()
	TMap<FGuid, FSkillData> AdditionalSkillData;

	/////////////////////////////函数/////////////////////////////////////////////
	//获取最大生命值
	float GetMaxHealth() const;
	//获取攻击力
	float GetAttack() const;
	//获取护甲
	float GetArmor() const;
	//获取CD
	float GetCD() const;
	//获取攻击速度
	float GetAttackSpeed() const;
};
