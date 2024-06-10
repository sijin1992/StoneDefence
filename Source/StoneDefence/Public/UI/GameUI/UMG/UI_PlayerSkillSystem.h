// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Drop/UI_NativeOnDrop.h"
#include "UI_PlayerSkillSystem.generated.h"

class UUI_SkillSlot;
class UHorizontalBox;
//class FKey;
/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UUI_PlayerSkillSystem : public UUI_NativeOnDrop
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* PlayerSkillList;
	
	//UPROPERTY(meta = (BindWidget))
	//UUI_SkillSlot *FreezeSkill;//冻结技能

	//UPROPERTY(meta = (BindWidget))
	//UUI_SkillSlot *ExplosionSkill;//爆炸技能

	//UPROPERTY(meta = (BindWidget))
	//UUI_SkillSlot *RecoverySkill;//恢复技能

	//UPROPERTY(meta = (BindWidget))
	//UUI_SkillSlot *RecoverMainTowersSkill;//恢复主塔技能

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUI_SkillSlot> PlayerSkillSlotClass;

public:

	virtual void NativeConstruct();

	void LayoutPlayerSkillSlot();

	void UpdatePlayerSkillSlot(const FGuid& PlayerSkillSlotGUID, bool bInCD);

	//FKey FreezeSkillKey;
	//FKey ExplosionSkillKey;
	//FKey RecoverySkillKey;
	//FKey RecoveryMainTowerSkillKey;
};
