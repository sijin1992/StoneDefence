// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSettings/UI_GameSettingsVideo.h"
#include "Core/SimpleGameUserSettings.h"
#include "UMG/Public/Components/ComboBoxString.h"
#include "UMG/Public/Components/CheckBox.h"
#include "UMG/Public/Components/SpinBox.h"
#include "SimpleGameSettingMacro.h"

//关闭优化
#if PLATFORM_WINDOWS
#pragma optimize("", off)
#endif

void UUI_GameSettingsVideo::NativeConstruct()
{
	Super::NativeConstruct();
	//调用基类的绑定
	Super::BindChangedValue(AntiAliasingSlider);
	Super::BindChangedValue(ShadowQualitySlider);
	Super::BindChangedValue(TextureQualitySlider);
	Super::BindChangedValue(PostProcessingSlider);
	Super::BindChangedValue(EffectsSlider);
	Super::BindChangedValue(FoliageSlider);
	Super::BindChangedValue(ViewDistanceSlider);
	Super::BindChangedValue(OverallScalabilityLevelSlider);

	Super::BindSelectionChanged(ResolutionBoxString);
	Super::BindSelectionChanged(LanguageString);
	
	FullScreenCheckBox->OnCheckStateChanged.AddDynamic(this, &UUI_GameSettingsVideo::FullScreenCheckBoxClicked);
	WindowScreenCheckBox->OnCheckStateChanged.AddDynamic(this, &UUI_GameSettingsVideo::WindowScreenCheckBoxClicked);
	SupportPhysXCheckBox->OnCheckStateChanged.AddDynamic(this, &UUI_GameSettingsVideo::SupportPhysXCheckBoxClicked);
	VSyncCheckBox->OnCheckStateChanged.AddDynamic(this, &UUI_GameSettingsVideo::VSyncCheckClickedBox);
	HDRDisplayCheckBox->OnCheckStateChanged.AddDynamic(this, &UUI_GameSettingsVideo::EnableHDRDisplayOutput);
	FrameRateLimitSpinBox->OnValueChanged.AddDynamic(this, &UUI_GameSettingsVideo::SetFrameRateLimit);
	//语言设置
	LanguageString->SetSelectedOption(USimpleGameUserSettings::GetSimpleGameUserSettings()->GetCurrentLanguageType());
}

void UUI_GameSettingsVideo::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUI_GameSettingsVideo::SaveSetting()
{
	Super::SetSettingsLevel(AntiAliasingSlider, AntiAliasingText,
		[](float InLevel)
		{
			USimpleGameUserSettings::GetSimpleGameUserSettings()->SetAntiAliasingQuality(InLevel * SCAL_ABILITY_QUALITY_LEVEL);
		});
	Super::SetSettingsLevel(ShadowQualitySlider, ShadowQualityText,
		[](float InLevel)
		{
			USimpleGameUserSettings::GetSimpleGameUserSettings()->SetShadowQuality(InLevel * SCAL_ABILITY_QUALITY_LEVEL);
		});
	Super::SetSettingsLevel(TextureQualitySlider, TextureQualityText,
		[](float InLevel)
		{
			USimpleGameUserSettings::GetSimpleGameUserSettings()->SetTextureQuality(InLevel * SCAL_ABILITY_QUALITY_LEVEL);
		});
	Super::SetSettingsLevel(PostProcessingSlider, PostProcessingText,
		[](float InLevel)
		{
			USimpleGameUserSettings::GetSimpleGameUserSettings()->SetPostProcessingQuality(InLevel * SCAL_ABILITY_QUALITY_LEVEL);
		});
	Super::SetSettingsLevel(EffectsSlider, EffectsText,
		[](float InLevel)
		{
			USimpleGameUserSettings::GetSimpleGameUserSettings()->SetVisualEffectQuality(InLevel * SCAL_ABILITY_QUALITY_LEVEL);
		});
	Super::SetSettingsLevel(FoliageSlider, FoliageText,
		[](float InLevel)
		{
			USimpleGameUserSettings::GetSimpleGameUserSettings()->SetFoliageQuality(InLevel * SCAL_ABILITY_QUALITY_LEVEL);
		});
	Super::SetSettingsLevel(ViewDistanceSlider, ViewDistanceText,
		[](float InLevel)
		{
			USimpleGameUserSettings::GetSimpleGameUserSettings()->SetViewDistanceQuality(InLevel * SCAL_ABILITY_QUALITY_LEVEL);
		});
}

void UUI_GameSettingsVideo::LoadSetting()
{
	EWindowMode::Type InModeType = USimpleGameUserSettings::GetSimpleGameUserSettings()->GetFullscreenMode();
	switch (InModeType)
	{
		case EWindowMode::WindowedFullscreen:
		case EWindowMode::Fullscreen:
			FullScreenCheckBox->SetCheckedState(ECheckBoxState::Checked);
			WindowScreenCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
			break;
		case EWindowMode::Windowed:
			FullScreenCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
			WindowScreenCheckBox->SetCheckedState(ECheckBoxState::Checked);
			break;
		case EWindowMode::NumWindowModes:
			break;
	}


	Super::LoadBoxString(ResolutionBoxString,
		[]() {
			FIntPoint ResolutionPoint = USimpleGameUserSettings::GetSimpleGameUserSettings()->GetScreenResolution();
			return  FString::Printf(TEXT("%ix%i"), ResolutionPoint.X, ResolutionPoint.Y);
		});
	Super::LoadBoxString(LanguageString,
		[]() {
			return USimpleGameUserSettings::GetSimpleGameUserSettings()->GetCurrentLanguageType();
		});
	Super::LoadSettingsLevel(AntiAliasingSlider, AntiAliasingText,
		[]() {
			//return USimpleGameUserSettings::GetSimpleGameUserSettings()->ScalabilityQuality.AntiAliasingQuality;
			return USimpleGameUserSettings::GetSimpleGameUserSettings()->GetAntiAliasingQuality() / (float)SCAL_ABILITY_QUALITY_LEVEL;
		});
	Super::LoadSettingsLevel(ShadowQualitySlider, ShadowQualityText,
		[]() {
			return USimpleGameUserSettings::GetSimpleGameUserSettings()->GetShadowQuality() / (float)SCAL_ABILITY_QUALITY_LEVEL;
		});
	Super::LoadSettingsLevel(TextureQualitySlider, TextureQualityText,
		[]() {
			return USimpleGameUserSettings::GetSimpleGameUserSettings()->GetTextureQuality() / (float)SCAL_ABILITY_QUALITY_LEVEL;
		});
	Super::LoadSettingsLevel(PostProcessingSlider, PostProcessingText,
		[]() {
			return USimpleGameUserSettings::GetSimpleGameUserSettings()->GetPostProcessingQuality() / (float)SCAL_ABILITY_QUALITY_LEVEL;
		});
	Super::LoadSettingsLevel(EffectsSlider, EffectsText,
		[]() {
			return USimpleGameUserSettings::GetSimpleGameUserSettings()->GetVisualEffectQuality() / (float)SCAL_ABILITY_QUALITY_LEVEL;
		});
	Super::LoadSettingsLevel(FoliageSlider, FoliageText,
		[]() {
			return USimpleGameUserSettings::GetSimpleGameUserSettings()->GetFoliageQuality() / (float)SCAL_ABILITY_QUALITY_LEVEL;
		});
	Super::LoadSettingsLevel(ViewDistanceSlider, ViewDistanceText,
		[]() {
			return USimpleGameUserSettings::GetSimpleGameUserSettings()->GetViewDistanceQuality() / (float)SCAL_ABILITY_QUALITY_LEVEL;
		});
	//Super::LoadSettingsLevel(OverallScalabilityLevelSlider, OverallScalabilityLevelSliderText,
	//	[]() {
	//		return USimpleGameUserSettings::GetSimpleGameUserSettings()->GetOverallScalabilityLevel();
	//	});
}

void UUI_GameSettingsVideo::FullScreenCheckBoxClicked(bool ClickedWidget)
{
	FullScreenCheckBox->SetCheckedState(ECheckBoxState::Checked);
	WindowScreenCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	USimpleGameUserSettings::GetSimpleGameUserSettings()->SetFullscreenMode(EWindowMode::Fullscreen);
}

void UUI_GameSettingsVideo::WindowScreenCheckBoxClicked(bool ClickedWidget)
{
	FullScreenCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	WindowScreenCheckBox->SetCheckedState(ECheckBoxState::Checked);
	USimpleGameUserSettings::GetSimpleGameUserSettings()->SetFullscreenMode(EWindowMode::Windowed);
}

void UUI_GameSettingsVideo::SupportPhysXCheckBoxClicked(bool ClickedWidget)
{
	//SupportPhysXCheckBox->SetCheckedState(ECheckBoxState::Checked);
}

void UUI_GameSettingsVideo::VSyncCheckClickedBox(bool ClickedWidget)
{
	USimpleGameUserSettings::GetSimpleGameUserSettings()->SetVSyncEnabled(ClickedWidget);
}

void UUI_GameSettingsVideo::EnableHDRDisplayOutput(bool ClickedWidget)
{
	USimpleGameUserSettings::GetSimpleGameUserSettings()->EnableHDRDisplayOutput(ClickedWidget, DisplayNitsSpinBox->GetValue());
}

void UUI_GameSettingsVideo::SetFrameRateLimit(float NewValue)
{
	USimpleGameUserSettings::GetSimpleGameUserSettings()->SetFrameRateLimit(NewValue);
}

void UUI_GameSettingsVideo::ChangeValue(float InValue)
{
	Super::UpdateAttributeLevel(AntiAliasingSlider, AntiAliasingText);
	Super::UpdateAttributeLevel(ShadowQualitySlider, ShadowQualityText);
	Super::UpdateAttributeLevel(TextureQualitySlider, TextureQualityText);
	Super::UpdateAttributeLevel(PostProcessingSlider, PostProcessingText);
	Super::UpdateAttributeLevel(EffectsSlider, EffectsText);
	Super::UpdateAttributeLevel(FoliageSlider, FoliageText);
	Super::UpdateAttributeLevel(ViewDistanceSlider, ViewDistanceText);
	Super::UpdateAttributeLevel(OverallScalabilityLevelSlider, OverallScalabilityLevelSliderText);
}

void UUI_GameSettingsVideo::SelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	//语言设置
	FString LanguageTemp = LanguageString->GetSelectedOption();
	USimpleGameUserSettings::GetSimpleGameUserSettings()->SetCurrentLanguage(LanguageTemp);

	//分辨率设置
	FIntPoint ResolutionPoint;
	FString ResolutionTemp = ResolutionBoxString->GetSelectedOption();
	FString R, L;
	ResolutionTemp.Split(TEXT("x"), &R, &L);
	ResolutionPoint.X = FCString::Atoi(*R);
	ResolutionPoint.Y = FCString::Atoi(*L);
	USimpleGameUserSettings::GetSimpleGameUserSettings()->SetScreenResolution(ResolutionPoint);
}

//打开优化
#if PLATFORM_WINDOWS
#pragma optimize("", on)
#endif
