#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BuildingTowerData.generated.h"

class UTexture2D;
//炮塔结构体
USTRUCT()
struct FBuildingTower
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(SaveGame)
	int32 TowerID;

	//需要花费的金币
	UPROPERTY(SaveGame)
	int32 NeedGold;

	//准备阶段建造数量
	UPROPERTY(SaveGame)
	int32 TowersPrepareBuildingNumber;

	//已经建造数量
	UPROPERTY(SaveGame)
	int32 TowersConstructionNumber;

	//当前塔最大建造CD
	UPROPERTY(SaveGame)
	float MaxConstructionTowersCD;

	//当前建造中的CD
	UPROPERTY(SaveGame)
	float CurrentConstructionTowersCD;

	//作为Icon指针
	UPROPERTY(SaveGame)
	UTexture2D* Icon;

	//是否正在被拖拽
	UPROPERTY(SaveGame)
	bool bDragIcon;

	//是否锁CD
	UPROPERTY(SaveGame)
	bool bLockCD;

	UPROPERTY(SaveGame)
	bool bCallUpdateTowersInfo;

public:
	FBuildingTower();

	void Init();

	//获取塔建造的百分比
	float GetTowerConstructionTimePercentage();

	bool IsValid();

	void ResetCD();
};