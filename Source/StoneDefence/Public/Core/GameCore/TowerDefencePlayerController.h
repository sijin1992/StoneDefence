// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Tool/ScreenMove.h"
#include "TowerDefencePlayerController.generated.h"

class ATowers;
class AMonsters;
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

public:
	//构建函数
	ATowerDefencePlayerController();
	
	//Tick
	virtual void Tick(float DeltaSeconds) override;

	//初始化
	virtual void BeginPlay() override;

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
