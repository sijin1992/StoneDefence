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

	//通知代理,如果是网络游戏的话需要加：UFUNCTION(Client)，代表这个逻辑是执行在客户端的
	UFUNCTION()
	void AddSkillSlot_Client(const FGuid& SlotID);//添加技能UI
	UFUNCTION()
	void SpawnBullet_Client(const FGuid& CharacterFGuid, UClass* InClass);//生成子弹特效

	void SetInputModeGameAndUI();

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

protected:

	FScreenMoveUnits ScreenMoveUnits;

	FHitResult MouseTraceHit;
};
