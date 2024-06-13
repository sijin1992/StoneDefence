// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_GameSettingsSystem.h"
#include "UMG/Public/Components/CheckBox.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "UMG/Public/Components/Button.h"
#include "GameSettings/UI_GameSettingsVideo.h"
#include "GameSettings/UI_GameSettingsAudio.h"
#include "GameSettings/UI_GameSettingsGame.h"
#include "Core/SimpleGameUserSettings.h"

void UUI_GameSettingsSystem::NativeConstruct()
{
	Super::NativeConstruct();
	VideoSettingBox->OnCheckStateChanged.AddDynamic(this, &UUI_GameSettingsSystem::VideoSettingCheckBox);
	AudioSettingBox->OnCheckStateChanged.AddDynamic(this, &UUI_GameSettingsSystem::AudioSettingCheckBox);
	GameSettingBox->OnCheckStateChanged.AddDynamic(this, &UUI_GameSettingsSystem::GameSettingCheckBox);

	SaveButton->OnClicked.AddDynamic(this, &UUI_GameSettingsSystem::SaveSetting);
	RestoreDefaultButton->OnClicked.AddDynamic(this, &UUI_GameSettingsSystem::RestoreDefaultSetting);
	//读取设置
	LoadSetting();
}

void UUI_GameSettingsSystem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUI_GameSettingsSystem::VideoSettingCheckBox(bool ClickedWidget)
{
	SetSettingState(EGameSettingType::VIDEO);
}

void UUI_GameSettingsSystem::AudioSettingCheckBox(bool ClickedWidget)
{
	SetSettingState(EGameSettingType::AUDIO);
}

void UUI_GameSettingsSystem::GameSettingCheckBox(bool ClickedWidget)
{
	SetSettingState(EGameSettingType::GAME);
}

void UUI_GameSettingsSystem::SaveSetting()
{
	GameSettingVideo->SaveSetting();
	GameSettingAudio->SaveSetting();
	GameSettingGame->SaveSetting();
	USimpleGameUserSettings::GetSimpleGameUserSettings()->ApplySettings(true);
	//刷新
	GameSettingVideo->LoadSetting();
	GameSettingAudio->LoadSetting();
	GameSettingGame->LoadSetting();
}

void UUI_GameSettingsSystem::LoadSetting()
{
	USimpleGameUserSettings::GetSimpleGameUserSettings()->LoadSettings(true);
	GameSettingVideo->LoadSetting();
	GameSettingAudio->LoadSetting();
	GameSettingGame->LoadSetting();
}

void UUI_GameSettingsSystem::RestoreDefaultSetting()
{
	USimpleGameUserSettings::GetSimpleGameUserSettings()->SetToDefaults();
	//刷新
	GameSettingVideo->LoadSetting();
	GameSettingAudio->LoadSetting();
	GameSettingGame->LoadSetting();
}

void UUI_GameSettingsSystem::SetSettingState(EGameSettingType SettingType)
{
	switch (SettingType)
	{
	case EGameSettingType::VIDEO:
		VideoSettingBox->SetCheckedState(ECheckBoxState::Checked);
		AudioSettingBox->SetCheckedState(ECheckBoxState::Unchecked);
		GameSettingBox->SetCheckedState(ECheckBoxState::Unchecked);
		break;
	case EGameSettingType::AUDIO:
		VideoSettingBox->SetCheckedState(ECheckBoxState::Unchecked);
		AudioSettingBox->SetCheckedState(ECheckBoxState::Checked);
		GameSettingBox->SetCheckedState(ECheckBoxState::Unchecked);
		break;
	case EGameSettingType::GAME:
		VideoSettingBox->SetCheckedState(ECheckBoxState::Unchecked);
		AudioSettingBox->SetCheckedState(ECheckBoxState::Unchecked);
		GameSettingBox->SetCheckedState(ECheckBoxState::Checked);
		break;
	}

	SettingListWitcher->SetActiveWidgetIndex((int32)SettingType);
}
