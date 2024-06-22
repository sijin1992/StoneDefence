// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameCore/TowerDefenceGameInstance.h"
#include "SimpleScreenLoading.h"
#include "Core/RuleOfTheGameState.h"
#include "Data/Save/GameSaveSlotList.h"
#include "Data/SimpleArchivesList.h"
#include "Kismet/GameplayStatics.h"
#include "../StoneDefenceUtils.h"
#include "Core/GameCore/TowerDefencePlayerState.h"
#include "Core/GameCore/TowerDefencePlayerController.h"

#define LOCTEXT_NAMESPACE "TowerGameInstance"

UTowerDefenceGameInstance::UTowerDefenceGameInstance()
	:ISimpleArchivesInterface(),
	SaveSlotNumber(INDEX_NONE)
{

}

void UTowerDefenceGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Init"));
	FSimpleScreenLoadingModule &SimpleScreenLoading = FModuleManager::LoadModuleChecked<FSimpleScreenLoadingModule>("SimpleScreenLoading");
	SimpleScreenLoading.SetupScreenLoading();
}

bool UTowerDefenceGameInstance::SaveGameData(int32 SaveNumber)
{
	bool bSave = false;
	//保存游戏数据
	if (ARuleOfTheGameState* InGameState = GetGameState())
	{
		if (FSaveSlot* InSlot = InGameState->GetSaveSlot(SaveNumber))
		{
			InSlot->DateText = FText::FromString(FDateTime::Now().ToString());
			InSlot->LevelName = LOCTEXT("LevelName", "TestMap");
			InSlot->ChapterName = LOCTEXT("ChapterName", "Hello World~~");
			InSlot->GameThumbnail = nullptr;

			bSave = InGameState->SaveGameData(SaveNumber);
		}
	}
	//保存玩家数据
	StoneDefenceUtils::CallUpdateAllClient(GetSafeWorld(), [&](ATowerDefencePlayerController* InPlayerController) 
		{
			if (ATowerDefencePlayerState* InState = InPlayerController->GetPlayerState<ATowerDefencePlayerState>())
			{
				bSave = InState->SaveGameData(SaveNumber);
			}
		});

	return bSave;
}

bool UTowerDefenceGameInstance::ReadGameData(int32 SaveNumber)
{
	return false;
}

bool UTowerDefenceGameInstance::ClearGameData(int32 SaveNumber)
{
	if (ARuleOfTheGameState* InGameState = GetGameState())
	{
		return InGameState->ClearGameData(SaveNumber);
	}
	return false;
}

int32 UTowerDefenceGameInstance::GetSaveSlotNumber() const
{
	int32 SlotNumber = 20;
	if (ARuleOfTheGameState *InGameState = GetGameState())
	{
		SlotNumber = InGameState->GetSaveSlotList().Slots.Num();
	}
	return SlotNumber;
}

FSaveSlot* UTowerDefenceGameInstance::GetSaveSlot(int32 SaveNumber)
{
	if (ARuleOfTheGameState* InGameState = GetGameState())
	{
		return InGameState->GetSaveSlot(SaveNumber);
	}
	return nullptr;
}

bool UTowerDefenceGameInstance::IsSlotValid(int32 SaveNumber) const
{
	if (ARuleOfTheGameState* InGameState = GetGameState())
	{
		if (FSaveSlot* InSlot = InGameState->GetSaveSlot(SaveNumber))
		{
			return InSlot->bSave;
		}
	}
	return false;
}

FSaveSlotList* UTowerDefenceGameInstance::GetSlotList()
{
	if (ARuleOfTheGameState* InGameState = GetGameState())
	{
		return &InGameState->GetSaveSlotList();
	}
	return nullptr;
}

bool UTowerDefenceGameInstance::OpenLevel(int32 SaveNumber)
{
	if (FSaveSlot* InSlot = GetSaveSlot(SaveNumber))
	{
		if (InSlot->bSave)
		{
			//储存存档是第几个
			SaveSlotNumber = SaveNumber;
			GameSaveType = EGameSaveType::ARCHIVES;
			OpenLevelOnServer(InSlot->LevelName);
			return true;
		}
	}
	return false;
}

int32 UTowerDefenceGameInstance::GetCurrentSaveSlotNumber() const
{
	return SaveSlotNumber;
}

void UTowerDefenceGameInstance::ClearSaveMark()
{
	SaveSlotNumber = INDEX_NONE;
	GameSaveType = EGameSaveType::NONE;
}

void UTowerDefenceGameInstance::SetCurrentSaveSlotNumber(int32 InSaveSlotNumber)
{
	SaveSlotNumber = InSaveSlotNumber;
}

void UTowerDefenceGameInstance::SetSaveNumber(int32 SaveNumber)
{
	SetCurrentSaveSlotNumber(SaveNumber);
}

class UWorld* UTowerDefenceGameInstance::GetSimpleWorld() const
{
	return GetWorld();
}

void UTowerDefenceGameInstance::OpenLevelOnServer(const FText& MapName)
{
	UGameplayStatics::OpenLevel(GetSafeWorld(), *MapName.ToString());
}

ARuleOfTheGameState* UTowerDefenceGameInstance::GetGameState() const
{
	return GetSafeWorld() == nullptr ? nullptr : GetSafeWorld()->GetGameState<ARuleOfTheGameState>();
}

UWorld* UTowerDefenceGameInstance::GetSafeWorld() const
{
	if (!GetWorld())
	{
		return GEngine->GetWorld();
	}
	return GetWorld();
}

#undef LOCTEXT_NAMESPACE