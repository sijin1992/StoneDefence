// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGameCore.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API USaveGameCore : public USaveGame
{
	GENERATED_BODY()

public:
	//创建时调用,如果是读取就不调用
	virtual void InitSaveGame(UWorld* InWorld);

	virtual void InitSaveGameFromArchives(UWorld* InWorld) {}
};
