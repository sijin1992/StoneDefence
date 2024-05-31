
#include "Data/Core/PlayerData.h"

FPlayerSkillData::FPlayerSkillData()
{
	Init();
}

void FPlayerSkillData::Init()
{
	SkillID = INDEX_NONE;
	SkillCD = 0.0f;
	MaxSkillID = 0.0f;
	SkillNumber = INDEX_NONE;
}

bool FPlayerSkillData::IsValid()
{
	return SkillID != INDEX_NONE;
}

FPlayerData::FPlayerData()
{
	Init();
}

void FPlayerData::Init()
{
	PlayID = INDEX_NONE;
	PlayName = NAME_None;
	GameGold = 0;
	Diamonds = 0;
	Copper = 0;
}

bool FPlayerData::IsValid()
{
	return PlayID != INDEX_NONE;
}
