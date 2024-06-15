// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/UI_SimpleGameSettingCore.h"
#include "UI_GameSettingsAudio.generated.h"

class USlider;
class UTextBlock;
class UButton;
class USoundBase;
/**
 * 
 */
UCLASS()
class SIMPLEGAMESETTINGS_API UUI_GameSettingsAudio : public UUI_SimpleGameSettingCore
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UButton* BackgroundSoundButton;//测试背景音乐开关

	UPROPERTY(meta = (BindWidget))
	UButton* GameSoundButton;//测试游戏声音开关

	UPROPERTY(meta = (BindWidget))
	UButton* UISoundButton;//测试UI声音开关

	UPROPERTY(meta = (BindWidget))
	USlider* AudioQualityLevelSlider;//音量设置

	UPROPERTY(meta = (BindWidget))
	USlider* BackgroundSoundSlider;//背景音乐设置

	UPROPERTY(meta = (BindWidget))
	USlider* GameSoundSlider;//游戏音乐设置

	UPROPERTY(meta = (BindWidget))
	USlider* UISoundSlider;//UI音乐设置

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AudioQualityLevelText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* BackgroundSoundText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* GameSoundText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* UISoundText;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* BackgroundSoundTestObject;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* GameSoundTestObject;

public:
	virtual void NativeConstruct() override;

	virtual void SaveSetting() override;
	virtual void LoadSetting() override;

	UFUNCTION()
	void SpawnBackgroundSoundForTest();

	void DestroyBackgroundSound();

	UFUNCTION()
	void SpawnGameSoundForTest();

	UFUNCTION()
	void SpawnUISoundForTest();

protected:
	virtual void ChangeValue(float InValue);
};
