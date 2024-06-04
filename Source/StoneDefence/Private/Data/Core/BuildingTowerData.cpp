#include "Data/Core/BuildingTowerData.h"

FBuildingTower::FBuildingTower()
{
	Init();
}

void FBuildingTower::Init()
{
	TowerID = INDEX_NONE;
	NeedGold = INDEX_NONE;
	TowersPrepareBuildingNumber = 0.0f;
	TowersConstructionNumber = 0.0f;
	MaxConstructionTowersCD = 0.0f;
	CurrentConstructionTowersCD = 0.0f;
	Icon = NULL;
	bDragIcon = false;
	bLockCD = false;
	bCallUpdateTowersInfo = false;
}

float FBuildingTower::GetTowerConstructionTimePercentage()
{
	return MaxConstructionTowersCD > 0.0f ? CurrentConstructionTowersCD / MaxConstructionTowersCD : 0.0f;
}

bool FBuildingTower::IsValid()
{
	return TowerID != INDEX_NONE;
}

void FBuildingTower::ResetCD()
{
	CurrentConstructionTowersCD = MaxConstructionTowersCD;
}