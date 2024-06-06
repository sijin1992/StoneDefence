#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DataCore.generated.h"

USTRUCT(BlueprintType)
struct FDataCore : public FTableRowBase
{
	GENERATED_BODY()

	FDataCore();

	virtual void Init();

	virtual bool IsValid() const;

	//ID
	UPROPERTY(EditDefaultsOnly, Category = "Attribute")
	int32 ID;
	//名字
	UPROPERTY(EditDefaultsOnly, Category = "Attribute")
	FName Name;
	//用游戏币购买的价格
	UPROPERTY(EditDefaultsOnly, Category = "Attribute")
	int32 GameGold;
	//用钻石购买的价格
	UPROPERTY(EditDefaultsOnly, Category = "Attribute")
	int32 Diamonds;
	//用铜币购买的价格
	UPROPERTY(EditDefaultsOnly, Category = "Attribute")
	int32 Copper;
};