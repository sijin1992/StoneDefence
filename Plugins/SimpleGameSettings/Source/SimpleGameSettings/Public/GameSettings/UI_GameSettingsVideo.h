// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/UI_SimpleGameSettingCore.h"
#include "UI_GameSettingsVideo.generated.h"

class USpinBox;
class UCheckBox;
class UComboBoxString;
class USlider;
class UTextBlock;
/**
 * 
 */
UCLASS()
class SIMPLEGAMESETTINGS_API UUI_GameSettingsVideo : public UUI_SimpleGameSettingCore
{
	GENERATED_BODY()

	///////////////////////设置///////////////////////////////////////////////////
	UPROPERTY(meta = (BindWidget))
	USpinBox* DisplayNitsSpinBox;//显示数字

	UPROPERTY(meta = (BindWidget))
	USpinBox* FrameRateLimitSpinBox;//帧率限制

	UPROPERTY(meta = (BindWidget))
	UCheckBox* FullScreenCheckBox;//全屏设置

	UPROPERTY(meta = (BindWidget))
	UCheckBox* WindowScreenCheckBox;//窗口设置

	UPROPERTY(meta = (BindWidget))
	UCheckBox* SupportPhysXCheckBox;//破碎效果开关

	UPROPERTY(meta = (BindWidget))
	UCheckBox* VSyncCheckBox;//垂直同步开关

	UPROPERTY(meta = (BindWidget))
	UCheckBox* HDRDisplayCheckBox;//HDR

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ResolutionBoxString;//分辨率设置

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* LanguageString;//语言设置

	UPROPERTY(meta = (BindWidget))
	USlider* AntiAliasingSlider;//抗锯齿设置

	UPROPERTY(meta = (BindWidget))
	USlider* ShadowQualitySlider;//阴影质量设置

	UPROPERTY(meta = (BindWidget))
	USlider* TextureQualitySlider;//贴图质量设置

	UPROPERTY(meta = (BindWidget))
	USlider* PostProcessingSlider;//后处理效果设置

	UPROPERTY(meta = (BindWidget))
	USlider* EffectsSlider;//特效效果设置

	UPROPERTY(meta = (BindWidget))
	USlider* FoliageSlider;//植被设置

	UPROPERTY(meta = (BindWidget))
	USlider* ViewDistanceSlider;//可视距离设置

	UPROPERTY(meta = (BindWidget))
	USlider* OverallScalabilityLevelSlider;//整体可扩展级别设置

	////////////////////////Text//////////////////////////////////////////////////

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AntiAliasingText;//锯齿Text

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ShadowQualityText;//阴影Text

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextureQualityText;//贴图质量Text

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PostProcessingText;//后处理Text

	UPROPERTY(meta = (BindWidget))
	UTextBlock* EffectsText;//特效效果Text

	UPROPERTY(meta = (BindWidget))
	UTextBlock* FoliageText;//植被Text

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ViewDistanceText;//可视距离Text

	UPROPERTY(meta = (BindWidget))
	UTextBlock* OverallScalabilityLevelSliderText;//整体可扩展级别Text

public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void SaveSetting() override;

	virtual void LoadSetting() override;

	UFUNCTION()
	void FullScreenCheckBoxClicked(bool ClickedWidget);//全屏

	UFUNCTION()
	void WindowScreenCheckBoxClicked(bool ClickedWidget);//窗口

	UFUNCTION()
	void SupportPhysXCheckBoxClicked(bool ClickedWidget);//是否支持物理

	UFUNCTION()
	void VSyncCheckClickedBox(bool ClickedWidget);

	UFUNCTION()
	void EnableHDRDisplayOutput(bool ClickedWidget);

	UFUNCTION()
	void SetFrameRateLimit(float NewValue);

protected:

	virtual void ChangeValue(float InValue) override;

	virtual void SelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType) override;
};
