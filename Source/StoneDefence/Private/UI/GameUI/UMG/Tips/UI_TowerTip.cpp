// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/Tips/UI_TowerTip.h"
#include "Data/CharacterData.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/MultiLineEditableTextBox.h"
#include "UMG/Public/Components/ProgressBar.h"

void UUI_TowerTip::InitTip(const FCharacterData& InData)
{
	LvBlock->SetText(FText::AsNumber(InData.Lv));
	CharacterNameBlock->SetText(FText::FromName(InData.Name));
	ConsumeGoldBlock->SetText(FText::AsNumber(InData.Gold));
	CharacterHealthBlock->SetText(FText::AsNumber(InData.GetMaxHealth()));
	CharacterAttackBlock->SetText(FText::AsNumber(InData.GetAttack()));
	CharacterArmorBlock->SetText(FText::AsNumber(InData.GetArmor()));
	CharacterAttackSpeedBlock->SetText(FText::AsNumber(InData.GetAttackSpeed()));
	IntroductionTextBox->SetText(InData.Introduction);
	EXPBar->SetPercent(InData.GetEXPPercent());
}