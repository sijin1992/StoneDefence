#include "Core/UI_SimpleGameSettingCore.h"
#include "UMG/Public/Components/Slider.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/ComboBoxString.h"
#include "SimpleGameSettingMacro.h"

void UUI_SimpleGameSettingCore::UpdateAttribute(USlider* NewSlider, UTextBlock* NewBlock)
{
	int32 IntNumber = NewSlider->GetValue() * SCAL_ABILITY_QUALITY_LEVEL;
	FString StringFloatNumberText = FString::Printf(TEXT(/*"%.2f"*/"%02d"), IntNumber);
	NewBlock->SetText(FText::FromString(StringFloatNumberText));

	//重置滑动条位置
	NewSlider->SetValue((float)IntNumber / (float)SCAL_ABILITY_QUALITY_LEVEL);
}

void UUI_SimpleGameSettingCore::BindChangedValue(USlider* NewSlider)
{
	NewSlider->OnValueChanged.AddDynamic(this, &UUI_SimpleGameSettingCore::ChangeValue);
}

void UUI_SimpleGameSettingCore::BindSelectionChanged(UComboBoxString* NewComboBoxString)
{
	NewComboBoxString->OnSelectionChanged.AddDynamic(this, &UUI_SimpleGameSettingCore::SelectionChanged);
}

void UUI_SimpleGameSettingCore::SetSettingsLevel(USlider* InSlider, UTextBlock* NewBlock, int32& InLevel)
{
	//默认级别
	InLevel = InSlider->GetValue() * SCAL_ABILITY_QUALITY_LEVEL;
	UpdateAttribute(InSlider, NewBlock);
}

void UUI_SimpleGameSettingCore::SetSettingsLevel(USlider* InSlider, UTextBlock* NewBlock, TFunction<void(int32 InLevel)> InFunc)
{
	//默认级别
	InFunc(InSlider->GetValue() * SCAL_ABILITY_QUALITY_LEVEL);
	UpdateAttribute(InSlider, NewBlock);
}

void UUI_SimpleGameSettingCore::LoadSettingsLevel(USlider* InSlider, UTextBlock* NewBlock, TFunction<float()> InFunc)
{
	InSlider->SetValue(InFunc() / (float)SCAL_ABILITY_QUALITY_LEVEL);
	UpdateAttribute(InSlider, NewBlock);
}

void UUI_SimpleGameSettingCore::LoadBoxString(UComboBoxString* InBoxString, TFunction<FString()> InFunc)
{
	InBoxString->SetSelectedOption(InFunc());
}
