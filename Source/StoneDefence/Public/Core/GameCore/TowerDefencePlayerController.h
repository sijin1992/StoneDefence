// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Tool/ScreenMove.h"
#include "TowerDefencePlayerController.generated.h"

class ATowers;
class AMonsters;
//代理
DECLARE_DELEGATE_OneParam(FAddSkillDelegate,FGuid)
DECLARE_DELEGATE_TwoParams(FSpawnBulletDelegate, FGuid, UClass*)
/**
 * 
 */
UCLASS()
class STONEDEFENCE_API ATowerDefencePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	//按下鼠标的代理
	FSimpleDelegate EventMouseMiddlePressed;
	//松开鼠标的代理
	FSimpleDelegate EventMouseMiddleReleased;
	//添加技能的代理
	FAddSkillDelegate AddSkillDelegate;
	//生成子弹的代理
	FSpawnBulletDelegate SpawnBulletDelegate;

public:
	//构建函数
	ATowerDefencePlayerController();
	
	//Tick
	virtual void Tick(float DeltaSeconds) override;

	//初始化
	virtual void BeginPlay() override;

	//如果是网络游戏的话需要加：UFUNCTION(Server)，S2C表示服务器通知客户端
	UFUNCTION(/*Server*/)
	void AddSkillSlot_S2C(const FGuid& CharacterFGuid, const FGuid& SlotID);//添加技能UI
	UFUNCTION(/*Server*/)
	void RemoveSkillSlot_S2C(const FGuid& CharacterFGuid, const FGuid& SlotID);//移除技能UI
	UFUNCTION(/*Client*/)
	void SpawnBullet_S2C(const FGuid& CharacterFGuid, const int32& SkillID);//生成子弹特效
	UFUNCTION(/*Client*/)
	void UpdateInventory_Client(const FGuid& InVentorySlotGUID, bool bInCD);//更新背包建造列表


	UFUNCTION(/*Client*/)
	void SpawnPlayerSkill_Client(const int32& PlayerSkillSlotGUID);//生成玩家技能
	UFUNCTION(/*Client*/)
	void UpdatePlayerSkill_Client(const FGuid& PlayerSkillSlotGUID, bool bInCD);//更新玩家技能


	//设置
	void SetInputModeGameAndUI();
	//更新全局变量
	void UpdateGlobalValue();

	/**
	* 绑定输入事件
	* @return:		
	*/
	virtual void SetupInputComponent() override;
	//鼠标滚轮
	void MouseWheelUp();
	void MouseWheelDown();

	//鼠标中键控制
	void MouseMiddleButtonPressed();
	void MouseMiddleButtonReleased();

	const FHitResult& GetHitResult();

/////////////////////////////////服务器GameMode相关/////////////////////////////////////////
	class AStoneDefenceGameMode* GetGameMode();

	//UFUNCTION(Server)表示该函数是客户端调用，运行在服务器上
	UFUNCTION()
	ATowers* SpawnTower(const int32 CharacterID, int32 CharacterLevel, const FVector& Location, const FRotator& Rotator);
	UFUNCTION()
	AMonsters* SpawnMonster(const int32 CharacterID, int32 CharacterLevel, const FVector& Location, const FRotator& Rotator);

	/////////////////////////////////GameState/////////////////////////////////////////
	class ATowerDefenceGameState* GetGameState();

protected:

	FScreenMoveUnits ScreenMoveUnits;

	FHitResult MouseTraceHit;
};
