// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UI_RuleOfTheWidget.h"
#include "UI_Health.generated.h"

class UTextBlock;
class UProgressBar;
class UWrapBox;
class UUI_CharacterSkillSlot;
/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UUI_Health : public UUI_RuleOfTheWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Title;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* Health;

	UPROPERTY(meta = (BindWidget))
	UWrapBox* SkillList;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUI_CharacterSkillSlot> CharacterSkillSlotClass;

public:
	virtual void NativeConstruct() override;

	void SetTitle(const FString& Msg);

	void SetHealth(float HealthValue);
	//添加技能Icon
	void AddSkillSlot(FGuid SkillFGuid);
	//移除技能Icon
	bool RemoveSkillSlot(FGuid SkillFGuid);
};
