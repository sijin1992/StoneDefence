// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "../Core/CharacterData.h"
#include "../Core/PlayerData.h"
#include "../Core/GameData.h"
#include "GameSaveData.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UGameSaveData : public USaveGame
{
	GENERATED_BODY()

public:
	//角色数据
	UPROPERTY(SaveGame)
	TMap<FGuid, FCharacterData> CharacterDatas;
	//游戏数据
	UPROPERTY(SaveGame)
	FGameInstanceDatas GameDatas;
};
