// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "RuleOfTheGameState.generated.h"

class UGameSaveSlotList;
struct FSaveSlotList;
struct FSaveSlot;
/**
 * 
 */
UCLASS()
class STONEDEFENCE_API ARuleOfTheGameState : public AGameState
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	FSaveSlotList& GetSaveSlotList();

	virtual FSaveSlot* GetSaveSlot(int32 SaveNumber);

	UFUNCTION(BlueprintCallable, Category = SaveData)
	virtual bool SaveGameData(int32 SaveNumber){return false;}

	UFUNCTION(BlueprintCallable, Category = SaveData)
	virtual bool ReadGameData(int32 SaveNumber){return false;}

	virtual bool ClearGameData(int32 SaveNumber);

protected:
	//存档数据
	UPROPERTY()
	UGameSaveSlotList* SlotList;

	//获取存档数据
	UGameSaveSlotList* GetGameSaveSlotList();
};
