// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSettings/UI_GameSettingsVideo.h"
#include "Core/SimpleGameUserSettings.h"
#include "UMG/Public/Components/ComboBoxString.h"
#include "UMG/Public/Components/CheckBox.h"
#include "UMG/Public/Components/SpinBox.h"

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
}

void UUI_GameSettingsVideo::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUI_GameSettingsVideo::SaveSetting()
{
	//SetSettingsLevel(TextureQualitySlider, USimpleGameUserSettings::GetSimpleGameUserSettings()->ScalabilityQuality.TextureQuality);
	Super::SetSettingsLevel(AntiAliasingSlider, AntiAliasingText,
		[](int32 InLevel)
		{
			USimpleGameUserSettings::GetSimpleGameUserSettings()->SetAntiAliasingQuality(InLevel);
			int32 value = USimpleGameUserSettings::GetSimpleGameUserSettings()->ScalabilityQuality.AntiAliasingQuality;
		});
	Super::SetSettingsLevel(ShadowQualitySlider, ShadowQualityText,
		[](int32 InLevel)
		{
			USimpleGameUserSettings::GetSimpleGameUserSettings()->SetShadowQuality(InLevel);
		});
	Super::SetSettingsLevel(TextureQualitySlider, TextureQualityText,
		[](int32 InLevel)
		{
			USimpleGameUserSettings::GetSimpleGameUserSettings()->SetTextureQuality(InLevel);
		});
	Super::SetSettingsLevel(PostProcessingSlider, PostProcessingText,
		[](int32 InLevel)
		{
			USimpleGameUserSettings::GetSimpleGameUserSettings()->SetPostProcessingQuality(InLevel);
		});
	Super::SetSettingsLevel(EffectsSlider, EffectsText,
		[](int32 InLevel)
		{
			USimpleGameUserSettings::GetSimpleGameUserSettings()->SetVisualEffectQuality(InLevel);
		});
	Super::SetSettingsLevel(FoliageSlider, FoliageText,
		[](int32 InLevel)
		{
			USimpleGameUserSettings::GetSimpleGameUserSettings()->SetFoliageQuality(InLevel);
		});
	Super::SetSettingsLevel(ViewDistanceSlider, ViewDistanceText,
		[](int32 InLevel)
		{
			USimpleGameUserSettings::GetSimpleGameUserSettings()->SetViewDistanceQuality(InLevel);
		});
	//Super::SetSettingsLevel(OverallScalabilityLevelSlider, OverallScalabilityLevelSliderText,
	//	[](int32 InLevel)
	//	{
	//		if (InLevel != USimpleGameUserSettings::GetSimpleGameUserSettings()->GetOverallScalabilityLevel())
	//		{
	//			USimpleGameUserSettings::GetSimpleGameUserSettings()->SetOverallScalabilityLevel(InLevel);
	//		}
	//	});
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
			return USimpleGameUserSettings::GetSimpleGameUserSettings()->GetAntiAliasingQuality();
		});
	Super::LoadSettingsLevel(ShadowQualitySlider, ShadowQualityText,
		[]() {
			return USimpleGameUserSettings::GetSimpleGameUserSettings()->GetShadowQuality();
		});
	Super::LoadSettingsLevel(TextureQualitySlider, TextureQualityText,
		[]() {
			return USimpleGameUserSettings::GetSimpleGameUserSettings()->GetTextureQuality();
		});
	Super::LoadSettingsLevel(PostProcessingSlider, PostProcessingText,
		[]() {
			return USimpleGameUserSettings::GetSimpleGameUserSettings()->GetPostProcessingQuality();
		});
	Super::LoadSettingsLevel(EffectsSlider, EffectsText,
		[]() {
			return USimpleGameUserSettings::GetSimpleGameUserSettings()->GetVisualEffectQuality();
		});
	Super::LoadSettingsLevel(FoliageSlider, FoliageText,
		[]() {
			return USimpleGameUserSettings::GetSimpleGameUserSettings()->GetFoliageQuality();
		});
	Super::LoadSettingsLevel(ViewDistanceSlider, ViewDistanceText,
		[]() {
			return USimpleGameUserSettings::GetSimpleGameUserSettings()->GetViewDistanceQuality();
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
	Super::UpdateAttribute(AntiAliasingSlider, AntiAliasingText);
	Super::UpdateAttribute(ShadowQualitySlider, ShadowQualityText);
	Super::UpdateAttribute(TextureQualitySlider, TextureQualityText);
	Super::UpdateAttribute(PostProcessingSlider, PostProcessingText);
	Super::UpdateAttribute(EffectsSlider, EffectsText);
	Super::UpdateAttribute(FoliageSlider, FoliageText);
	Super::UpdateAttribute(ViewDistanceSlider, ViewDistanceText);
	Super::UpdateAttribute(OverallScalabilityLevelSlider, OverallScalabilityLevelSliderText);
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
