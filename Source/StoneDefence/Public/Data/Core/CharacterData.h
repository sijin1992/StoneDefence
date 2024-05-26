// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SkillData.h"
#include "CharacterData.generated.h"

USTRUCT(BlueprintType)
struct FCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FCharacterData();
	//////////////////////////////////基础属性////////////////////////////////////////
	//玩家ID
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Table ID")
	FString GUID;

	//角色名字
	UPROPERTY(EditDefaultsOnly, Category = "Table Attribute")
	FName Name;

	//角色等级
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Table Attribute")
	float Lv;

	//最大生命值
	UPROPERTY(EditDefaultsOnly, Category = "Table Attribute")
	float MaxHealth;

	//当前生命值
	UPROPERTY()
	float Health;

	//基础攻击
	UPROPERTY(EditDefaultsOnly, Category = "Table Attribute")
	float PhysicalAttack;

	//护甲
	UPROPERTY(EditDefaultsOnly, Category = "Table Attribute")
	float Armor;

	//最大经验值
	UPROPERTY(EditDefaultsOnly, Category = "Table Attribute")
	float MaxEmpircalValue;

	//经验值
	UPROPERTY()
	float EmpirceValue;

	//CD
	UPROPERTY(EditDefaultsOnly, Category = "Table Attribute")
	float CD;

	//攻击速度
	UPROPERTY(EditDefaultsOnly, Category = "Table Attribute")
	float AttackSpeed;

	//如果是塔，则代表花费多少金币可以建造
	//如果是怪物，代码消灭它可以获取多少金币
	UPROPERTY(EditDefaultsOnly, Category = "Table Attribute")
	float Gold;

	//角色简介
	UPROPERTY(EditDefaultsOnly, Category = "Table Attribute")
	FText Introduction;

	
	///////////////////////////增益部分///////////////////////////////////////////////
	//添加金币
	UPROPERTY(EditDefaultsOnly, Category = "Table Profit")
	float AddGold;

	//添加生命值
	UPROPERTY(EditDefaultsOnly, Category = "Table Profit")
	float AddHealth;

	//添加攻击力
	UPROPERTY(EditDefaultsOnly, Category = "Table Profit")
	float AddPhysicalAttack;

	//添加护甲
	UPROPERTY(EditDefaultsOnly, Category = "Table Profit")
	float AddArmor;

	//添加经验值
	UPROPERTY(EditDefaultsOnly, Category = "Table Profit")
	float AddEmpiricalValue;

	//添加攻击速度
	UPROPERTY(EditDefaultsOnly, Category = "Table Profit")
	float AddAttackSpeed;

	//自动恢复生命值
	UPROPERTY(EditDefaultsOnly, Category = "Table Attribute")
	float RestoreHealth;

	//////////////////////////////塔的技能////////////////////////////////////////////

	//添加被动生命值
	UPROPERTY(EditDefaultsOnly, Category = "Table Passive")
	float AddPassiveSkillHealth;
	
	//添加持续恢复生命值
	UPROPERTY(EditDefaultsOnly, Category = "Table Passive")
	float AddContinueHealth;

	//添加被动攻击力
	UPROPERTY(EditDefaultsOnly, Category = "Table Passive")
	float AddPassiveSkillPhysicalAttack;

	//添加被动护甲
	UPROPERTY(EditDefaultsOnly, Category = "Table Passive")
	float AddPassiveSkilldArmor;

	//添加被动攻击速度
	UPROPERTY(EditDefaultsOnly, Category = "Table Passive")
	float AddPassiveSkillAttackSpeed;

	//减少被动CD
	UPROPERTY(EditDefaultsOnly, Category = "Table Passive")
	float ReducePassiveSkillCDTime;

	//角色多久建立好
	UPROPERTY(EditDefaultsOnly, Category = "Table Build")
	float ConstructionTime;

	/////////////////////////////技能/////////////////////////////////////////////
	UPROPERTY(EditDefaultsOnly, Category = "Table Skill")
	TMap<int32, FSkillData> CharacterSkill;
};
