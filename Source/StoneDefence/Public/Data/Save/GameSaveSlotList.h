// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameSaveSlotList.generated.h"


USTRUCT()
struct FSaveSlot
{
	GENERATED_USTRUCT_BODY()

	FSaveSlot();

	//存档日期
	UPROPERTY(SaveGame)
	FText DataText;

	//存档下标
	UPROPERTY(SaveGame)
	int32 SaveGameBoxNumber;

};


USTRUCT()
struct FSaveSlotList
{
	GENERATED_USTRUCT_BODY()

	TArray<FSaveSlot> Slots;
};