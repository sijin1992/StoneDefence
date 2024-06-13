// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "SimpleGameSettingType.h"
#include "Core/UI_SimpleGameSettingCore.h"
#include "UI_GameSettingsSystem.generated.h"

class UCheckBox;
class UButton;
class UWidgetSwitcher;
class UUI_GameSettingsVideo;
class UUI_GameSettingsAudio;
class UUI_GameSettingsGame;
/**
 * 
 */
UCLASS()
class SIMPLEGAMESETTINGS_API UUI_GameSettingsSystem : public UUI_SimpleGameSettingCore
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UCheckBox* VideoSettingBox;//图像设置

	UPROPERTY(meta = (BindWidget))
	UCheckBox* AudioSettingBox;//声音设置

	UPROPERTY(meta = (BindWidget))
	UCheckBox* GameSettingBox;//游戏设置

	UPROPERTY(meta = (BindWidget))
	UButton* SaveButton;//保存设置按钮

	UPROPERTY(meta = (Bindwidget))
	UButton* RestoreDefaultButton;//恢复默认按钮

	UPROPERTY(meta = (Bindwidget))
	UWidgetSwitcher* SettingListWitcher;//页签切换器

	UPROPERTY(meta = (BindWidget))
	UUI_GameSettingsVideo* GameSettingVideo;

	UPROPERTY(meta = (BindWidget))
	UUI_GameSettingsAudio* GameSettingAudio;

	UPROPERTY(meta = (BindWidget))
	UUI_GameSettingsGame* GameSettingGame;

public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void VideoSettingCheckBox(bool ClickedWidget);

	UFUNCTION()
	void AudioSettingCheckBox(bool ClickedWidget);

	UFUNCTION()
	void GameSettingCheckBox(bool ClickedWidget);

	virtual void SaveSetting() override;

	virtual void LoadSetting() override;

	UFUNCTION()
	void RestoreDefaultSetting();

private:
	
	void SetSettingState(EGameSettingType SettingType);
};
