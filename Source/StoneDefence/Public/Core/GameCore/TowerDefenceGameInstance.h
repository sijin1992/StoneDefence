// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interface/SimpleArchivesInterface.h"
#include "TowerDefenceGameInstance.generated.h"

class ARuleOfTheGameState;
struct FSaveSlotList;
/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UTowerDefenceGameInstance : public UGameInstance, public ISimpleArchivesInterface
{
	GENERATED_BODY()

public:
	UTowerDefenceGameInstance();

	virtual void Init() override;

	virtual bool SaveGameData(int32 SaveNumber) override;

	virtual bool ReadGameData(int32 SaveNumber) override;

	virtual bool ClearGameData(int32 SaveNumber) override;

	virtual int32 GetSaveSlotNumber() const override;

	ARuleOfTheGameState* GetGameState() const;

	class UWorld* GetSafeWorld() const;

	virtual FSaveSlot* GetSaveSlot(int32 SaveNumber) override;

	virtual bool IsSlotValid(int32 SaveNumber) const override;

	virtual FSaveSlotList* GetSlotList() override;

	virtual bool OpenLevel(int32 SaveNumber) override;

	int32 GetCurrentSaveSlotNumber() const;


	void ClearSaveMark();

	UFUNCTION(/*Server*/)
	void SetCurrentSaveSlotNumber(int32 InSaveSlotNumber);


	virtual void SetSaveNumber(int32 SaveNumber) override;


	virtual UWorld* GetSimpleWorld() const override;

	EGameSaveType GetGameType() { return GameSaveType; };

protected:
	UPROPERTY()
	int32 SaveSlotNumber;

	//游戏存储状态flag
	EGameSaveType GameSaveType;

	//通知服务器打开地图
	UFUNCTION(/*Server , Replite*/)
	void OpenLevelOnServer(const FText& MapName);

};
