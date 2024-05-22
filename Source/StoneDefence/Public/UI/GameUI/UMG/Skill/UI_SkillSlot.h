// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UI_RuleOfTheWidget.h"
#include "UI_SkillSlot.generated.h"

class UImage;
class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UUI_SkillSlot : public UUI_RuleOfTheWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UImage* SkillIcon;//技能图标

	UPROPERTY(meta = (BindWidget))
	UImage* SkillIconCD;//技能CD蒙版

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SkillNumber;//技能次数

	UPROPERTY(meta = (BindWidget))
	UTextBlock* KeyValueNumber;//技能按键

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SkillCDValue;//技能CD

	UPROPERTY(meta = (BindWidget))
	UButton* ClickButton;//敲击按钮

public:
	virtual void NativeConstruct();

	UFUNCTION()
	void OnClickedWidget();
};
