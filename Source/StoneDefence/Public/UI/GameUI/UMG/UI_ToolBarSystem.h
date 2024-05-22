// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UI_RuleOfTheWidget.h"
#include "UI_ToolBarSystem.generated.h"

class UTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UUI_ToolBarSystem : public UUI_RuleOfTheWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* GameGlob;//金币数量

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TowersDeathNumber;//塔死亡数

	UPROPERTY(meta = (BindWidget))
	UTextBlock* GameCount;//游戏计时

	UPROPERTY(meta = (BindWidget))
	UTextBlock* KillSoldier;//击杀士兵

	UPROPERTY(meta = (BindWidget))
	UTextBlock* GameLevelSurplusQuantity;//剩余敌人数量

	UPROPERTY(meta = (BindWidget))
	UProgressBar* GSQProgressBar;//剩余敌人数量进度条

public:
	virtual void NativeConstruct();
};
