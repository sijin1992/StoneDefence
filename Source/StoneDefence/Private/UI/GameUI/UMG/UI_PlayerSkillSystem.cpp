// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/UI_PlayerSkillSystem.h"
#include "GameFramework/InputSettings.h"
#include "UMG/Public/Components/HorizontalBox.h"
#include "UI/GameUI/UMG/Skill/UI_SkillSlot.h"
#include "UMG/Public/Components/HorizontalBoxSlot.h"

void UUI_PlayerSkillSystem::NativeConstruct()
{
	Super::NativeConstruct();

	//auto KeyMapping = GetDefault<UInputSettings>()->ActionMappings.FindByPredicate([&](FInputActionKeyMapping& EntryUI) { return (EntryUI.ActionName == "FreezeSkill"); });

	LayoutPlayerSkillSlot();
}

void UUI_PlayerSkillSystem::LayoutPlayerSkillSlot()
{
	if (PlayerSkillSlotClass)
	{
		const TArray<const FGuid*> PlayerSkillGUIDs = GetPlayerState()->GetPlayerSkillIDs();
		for (int32 i = 0; i < PlayerSkillGUIDs.Num(); i++)
		{
			if (UUI_SkillSlot* PlayerSkillSlot = CreateWidget<UUI_SkillSlot>(GetWorld(), PlayerSkillSlotClass))
			{
				PlayerSkillSlot->GUID = *PlayerSkillGUIDs[i];
				if (UHorizontalBoxSlot* PanelSlot = PlayerSkillList->AddChildToHorizontalBox(PlayerSkillSlot))
				{
					PanelSlot->SetPadding(20.0f);
					PanelSlot->SetSize(ESlateSizeRule::Fill);
					PanelSlot->SetHorizontalAlignment(HAlign_Fill);
					PanelSlot->SetVerticalAlignment(VAlign_Fill);
				}

				//技能赋值
				if (GetPlayerState()->GetPlayerData().SkillID.IsValidIndex(i))
				{
					int32 CurrentSkillID = GetPlayerState()->GetPlayerData().SkillID[i];
					GetPlayerState()->AddPlayerSkill(*PlayerSkillGUIDs[i], CurrentSkillID);
				}

				PlayerSkillSlot->UpdateUI();
			}
		}
	}

}

void UUI_PlayerSkillSystem::UpdatePlayerSkillSlot(const FGuid& PlayerSkillSlotGUID, bool bInCD)
{
	for (UPanelSlot* PanelSlot : PlayerSkillList->GetSlots())
	{
		if (UUI_SkillSlot* SkillSlot = Cast<UUI_SkillSlot>(PanelSlot->Content))
		{
			if (SkillSlot->GUID == PlayerSkillSlotGUID)
			{
				if (bInCD)
				{
					SkillSlot->DrawSlotCD(SkillSlot->GetPlayerSkillData()->GetCDPercent());
				}
				else
				{
					SkillSlot->DrawSlotCD(0.0f);
				}
				SkillSlot->UpdateSlotInfo(SkillSlot->GetPlayerSkillData()->CDTime, SkillSlot->GetPlayerSkillData()->SkillNumber);
				break;
			}
		}
	}
}
