// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Core/CharacterData.h"

FCharacterData::FCharacterData()
	: ID(INDEX_NONE),//-1
	Name(NAME_None),
	Lv(1),
	MaxHealth(100),
	Health(MaxHealth),
	PhysicalAttack(10),
	Armor(10),
	MaxEmpircalValue(100),
	EmpirceValue(0.0f),
	CD(2.0f),
	AttackSpeed(0.66),
	Gold(80),

	AddGold(30),
	AddHealth(50.0f),
	AddPhysicalAttack(10.0f),
	AddArmor(8.0f),
	AddEmpiricalValue(100),
	AddAttackSpeed(0.001f),
	RestoreHealth(0.2f),

	AddPassiveSkillHealth(0.0f),
	AddContinueHealth(0.0f),
	AddPassiveSkillPhysicalAttack(0.0f),
	AddPassiveSkilldArmor(0),
	AddPassiveSkillAttackSpeed(0),
	ReducePassiveSkillCDTime(0.45f),
	ConstructionTime(5)
{

}

bool FCharacterData::IsValid() const
{
	return ID != INDEX_NONE;
}

void FCharacterData::UpdateHealth()
{
	Health = MaxHealth;
}

bool FCharacterData::UpdateEXP(float InExp)
{
	EmpirceValue += InExp;
	if (EmpirceValue >= MaxEmpircalValue)//升极
	{
		EmpirceValue -= MaxEmpircalValue;

		UpdateLevel();

		return true;
	}
	return false;
}

float FCharacterData::GetEXPPercent() const
{
	if (MaxEmpircalValue != 0.0F)
	{
		return EmpirceValue / MaxEmpircalValue;
	}
	return 0.0f;
}

void FCharacterData::UpdateLevel()
{
	//被动技能加成
	float Coefficient = 0.1f;

	Lv += 1;
	Gold += (Lv - 1) * AddHealth * Coefficient;

	//属性提升
	MaxHealth += (Lv - 1) * AddHealth * Coefficient;
	PhysicalAttack += (Lv - 1) * AddPhysicalAttack * Coefficient;
	AttackSpeed += (Lv - 1) * AddAttackSpeed * Coefficient;
	Armor += (Lv - 1) * AddArmor * Coefficient;
	MaxEmpircalValue += (Lv - 1) * AddEmpiricalValue * Coefficient;
	RestoreHealth += (RestoreHealth * Lv) / 100;

	//被动技能
	AddPassiveSkillHealth += ((Lv - 1) * AddPassiveSkillHealth) * (Coefficient - 0.09f);
	AddPassiveSkillPhysicalAttack += (Lv - 1) * AddPassiveSkillPhysicalAttack * (Coefficient - 0.09f);
	AddPassiveSkillAttackSpeed += (Lv - 1) * AddPassiveSkillAttackSpeed * (Coefficient - 0.09f);
	AddPassiveSkilldArmor += (Lv - 1) * AddPassiveSkilldArmor * (Coefficient - 0.09f);

	Health = MaxHealth;
}
