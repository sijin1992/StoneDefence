
#include "Data/PlayerData.h"

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
	Team = ETeam::RED;
	GameGoldTime = 0.0f;
	MaxGameGoldTime = 1.24f;
}

bool FPlayerData::IsValid()
{
	return PlayID != INDEX_NONE;
}

bool FPlayerData::IsAllowIncrease()
{
	return GameGoldTime >= MaxGameGoldTime;
}
