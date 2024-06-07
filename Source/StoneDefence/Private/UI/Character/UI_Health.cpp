// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Character/UI_Health.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/ProgressBar.h"
#include "UMG/Public/Components/WrapBox.h"
#include "UI/GameUI/UMG/Skill/UI_CharacterSkillSlot.h"

void UUI_Health::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI_Health::SetTitle(const FString& Msg)
{
	Title->SetText(FText::FromString(Msg));
}

void UUI_Health::SetHealth(float HealthValue)
{
	Health->SetPercent(HealthValue);
}

void UUI_Health::AddSkillSlot(FGuid SkillFGuid)
{
	if (CharacterSkillSlotClass)
	{
		if (UUI_CharacterSkillSlot* SkillSlot = CreateWidget<UUI_CharacterSkillSlot>(GetWorld(), CharacterSkillSlotClass))
		{
			SkillSlot->GUID = SkillFGuid;
			if (SkillSlot->GetSkillData().IsValid())
			{
				UTexture2D* IconTexture = SkillSlot->GetSkillData().Icon.LoadSynchronous();
				SkillSlot->SetTexture(IconTexture);
				SkillList->AddChild(SkillSlot);
			}
		}
	}
}

bool UUI_Health::RemoveSkillSlot(FGuid SkillFGuid)
{
	UUI_CharacterSkillSlot* RemoveSkillSlot = nullptr;
	for (UPanelSlot* PanelSlot : SkillList->GetSlots())
	{
		if (UUI_CharacterSkillSlot* SkillSlot = Cast<UUI_CharacterSkillSlot>(PanelSlot->Content))
		{
			if (SkillSlot->GUID == SkillFGuid)
			{
				RemoveSkillSlot = SkillSlot;
				break;
			}
		}
	}
	return SkillList->RemoveChild(RemoveSkillSlot);
}
