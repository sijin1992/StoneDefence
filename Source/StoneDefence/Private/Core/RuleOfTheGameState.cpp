// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/RuleOfTheGameState.h"
#include "Data/Save/GameSaveSlotList.h"
#include "Kismet/GameplayStatics.h"
#include "../StoneDefenceUtils.h"

void ARuleOfTheGameState::BeginPlay()
{
	Super::BeginPlay();
}

FSaveSlotList& ARuleOfTheGameState::GetSaveSlotList()
{
	return GetGameSaveSlotList()->SlotList;
}

FSaveSlot* ARuleOfTheGameState::GetSaveSlot(int32 SaveNumber)
{
	if (GetSaveSlotList().Slots.Contains(SaveNumber))
	{
		return &GetSaveSlotList().Slots[SaveNumber];
	}
	return nullptr;
}

bool ARuleOfTheGameState::ClearGameData(int32 SaveNumber)
{
	if (FSaveSlot* InSlot = GetSaveSlot(SaveNumber))
	{
		InSlot->Init();
		return true;
	}
	return false;
}

UGameSaveSlotList* ARuleOfTheGameState::GetGameSaveSlotList()
{
	if (!SlotList)
	{
		SlotList = StoneDefenceUtils::GetSave<UGameSaveSlotList>(
			GetWorld(), 
			TEXT("SlotList"),
			INDEX_NONE,
			EGameSaveType::ARCHIVES);
	}
	return SlotList;
}
