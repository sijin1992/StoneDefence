// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/Skill/UI_CharacterSkillSlot.h"
#include "UMG/Public/Components/Border.h"
#include "UMG/Public/Components/Image.h"
#include "Data/SkillData.h"
#include "Materials/MaterialInstanceDynamic.h"

void UUI_CharacterSkillSlot::NativeConstruct()
{
	Super::NativeConstruct();
	CDMaterialDynamic = CD->GetDynamicMaterial();
}

void UUI_CharacterSkillSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	FSkillData& SkillData = GetSkillData();
	if (SkillData.IsValid())
	{
		UpdateCD(SkillData.GetDurationPercent());
	}
}

void UUI_CharacterSkillSlot::SetTexture(UTexture2D* InTexture)
{
	Icon->SetBrushFromTexture(InTexture);
}

void UUI_CharacterSkillSlot::UpdateCD(float SkillCD)
{
	if (CDMaterialDynamic)
	{
		if (SkillCD > 0.0f && SkillCD < 1.0f)
		{
			CDMaterialDynamic->SetScalarParameterValue("ClearCD", true);
			CD->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			CDMaterialDynamic->SetScalarParameterValue("ClearCD", false);
			CD->SetVisibility(ESlateVisibility::Hidden);
		}

		CDMaterialDynamic->SetScalarParameterValue("CDValue", SkillCD);
	}
}

FSkillData& UUI_CharacterSkillSlot::GetSkillData()
{
	return GetGameState()->GetSkillData(GUID);
}
