// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UI_RuleOfTheWidget.h"
#include "UI_TowerTip.generated.h"

class UTextBlock;
class UMultiLineEditableTextBox;
struct FCharacterData;
/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UUI_TowerTip : public UUI_RuleOfTheWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LvBlock;//名字
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CharacterNameBlock;//名字

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ConsumeGoldBlock;//销毁的金币

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CharacterHealthBlock;//生命值

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CharacterAttackBlock;//攻击力

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CharacterArmorBlock;//防御力

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CharacterAttackSpeedBlock;//攻击速度

	UPROPERTY(meta = (BindWidget))
	UMultiLineEditableTextBox* IntroductionTextBox;//支持多行介绍

public:

	void InitTip(const FCharacterData& InData);
};
