// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/UI_Slot.h"
#include "UI_SkillSlot.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UUI_SkillSlot : public UUI_Slot
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Number;//技能按键:数字键

public:
	virtual void NativeConstruct() override;

	virtual void OnClickedWidget() override;

	FORCEINLINE int32 GetKeyNumber() const { return KeyNumber; }

	FPlayerSkillData* GetPlayerSkillData();

	void UpdateUI();

private:
	int32 KeyNumber;
};
