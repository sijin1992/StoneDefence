// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Tool/ScreenMove.h"
#include "TowerDefencePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API ATowerDefencePlayerController : public APlayerController
{
	GENERATED_BODY()

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

	void MouseWheelUp();
	void MouseWheelDown();

protected:

	FScreenMoveUnits ScreenMoveUnits;
};
