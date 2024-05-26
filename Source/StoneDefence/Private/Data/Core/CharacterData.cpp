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
	EmpirceValue(MaxEmpircalValue),
	CD(2.0f),
	AttackSpeed(0.66),
	Gold(80),

	AddGold(30),
	AddHealth(0.0f),
	AddPhysicalAttack(0.0f),
	AddArmor(0.0f),
	AddEmpiricalValue(20),
	AddAttackSpeed(0.0f),
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

bool FCharacterData::IsValid()
{
	return ID != INDEX_NONE;
}
