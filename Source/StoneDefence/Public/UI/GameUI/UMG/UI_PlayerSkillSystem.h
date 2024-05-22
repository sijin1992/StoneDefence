// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UI_RuleOfTheWidget.h"
#include "UI_PlayerSkillSystem.generated.h"

class UUI_SkillSlot;
//class FKey;
/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UUI_PlayerSkillSystem : public UUI_RuleOfTheWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UUI_SkillSlot *FreezeSkill;//冻结技能

	UPROPERTY(meta = (BindWidget))
	UUI_SkillSlot *ExplosionSkill;//爆炸技能

	UPROPERTY(meta = (BindWidget))
	UUI_SkillSlot *RecoverySkill;//恢复技能

	UPROPERTY(meta = (BindWidget))
	UUI_SkillSlot *RecoverMainTowersSkill;//恢复主塔技能

public:

	virtual void NativeConstruct();

	FKey FreezeSkillKey;
	FKey ExplosionSkillKey;
	FKey RecoverySkillKey;
	FKey RecoveryMainTowerSkillKey;
};
