// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "../Core/CharacterData.h"
#include "GameSaveSlotList.generated.h"


USTRUCT()
struct FSaveSlot
{
	GENERATED_USTRUCT_BODY()

	FSaveSlot();

	//存档日期
	UPROPERTY(SaveGame)
	FText DataText;

	//是否有存档
	UPROPERTY(SaveGame)
	bool bSave;

};


USTRUCT()
struct FSaveSlotList
{
	GENERATED_USTRUCT_BODY()

	FSaveSlotList();

	UPROPERTY(SaveGame)
	TMap<int32, FSaveSlot> Slots;//存档列表
	UPROPERTY(SaveGame)
	TMap<int32, float> DegreeOfCompletion;//关卡完成度
	//获得空闲存档的下标
	int32 GetSerialNumber();
	//删除存档
	bool RemoveAtGameData(int32 SlotNumber);
	//添加存档
	bool AddGameDataByNumber(int32 SlotNumber);
};


/**
 *
 */
UCLASS()
class STONEDEFENCE_API UGameSaveSlotList : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame)
	FSaveSlotList SlotList;
};