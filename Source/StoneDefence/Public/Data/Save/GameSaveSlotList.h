// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/SaveGameCore.h"
#include "../CharacterData.h"
#include "Data/SimpleArchivesList.h"
#include "GameSaveSlotList.generated.h"

/**
 *
 */
UCLASS()
class STONEDEFENCE_API UGameSaveSlotList : public USaveGameCore
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame)
	FSaveSlotList SlotList;

	virtual void InitSaveGame(UWorld* InWorld) override;
};