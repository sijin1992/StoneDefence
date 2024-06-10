// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/Skill/UI_SkillSlot.h"
#include "Global/UI_Datas.h"
#include "Data/PlayerSkillData.h"

void UUI_SkillSlot::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (PlayerSkillNumber == 9)
	{
		PlayerSkillNumber = 0;
		Number->SetText(FText::FromString(FString::FromInt(PlayerSkillNumber)));
	}
	else
	{
		Number->SetText(FText::FromString(FString::FromInt(++PlayerSkillNumber)));
	}
	KeyNumber = PlayerSkillNumber;
	//映射按键
	FString PlayerSkillName = FString::Printf(TEXT("PlayerSkill_%i"), PlayerSkillNumber);
	GetPlayerController()->InputComponent->BindAction(*PlayerSkillName, IE_Pressed, this, &UUI_SkillSlot::OnClickedWidget);
}

void UUI_SkillSlot::OnClickedWidget()
{
	if (GetPlayerState()->IsVerificationSkill(GUID))
	{
		GetPlayerState()->UsePlayerSkill(GUID);
	}
}

FPlayerSkillData* UUI_SkillSlot::GetPlayerSkillData()
{
	return GetPlayerState()->GetPlayerSkillData(GUID);
}

void UUI_SkillSlot::UpdateUI()
{
	Super::UpdateSlotUI(GetPlayerSkillData()->Icon.LoadSynchronous(), GetPlayerSkillData()->SkillNumber);
}
