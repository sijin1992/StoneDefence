#include "Data/SkillData.h"

FSkillData::FSkillData()
	:Super()
{
}

void FSkillData::Init()
{
	Super::Init();

	Gold = 0;
	Health = 0.0f;
	PhysicalAttack = 0.0f;
	Armor = 0.0f;
	AttackRange = 0.0f;
	MaxSkillDuration = 0.0f;
	SkillDuration = 0.0f;
	SkillDurationTime = 0.0f;
	CD = 0.0f;
}

float FSkillData::GetCDPercent() const
{
	return CD == 0.0f ? 0.0f : (CDTime / CD);
}

void FSkillData::ResetCD()
{
	CDTime = CD;
}

float FSkillData::GetDurationPercent() const
{
	return MaxSkillDuration == 0.0f ? 0.0f : (SkillDuration / MaxSkillDuration);
}

void FSkillData::ResetDuration()
{
	SkillDuration = MaxSkillDuration;
}
