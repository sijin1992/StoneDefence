
#pragma once

#include "CoreMinimal.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "SimpleGameSettingType.h"
#include "UI_SimpleGameSettingCore.generated.h"

class USlider;
class UTextBlock;
class UComboBoxString;
/**
 *
 */
UCLASS()
class SIMPLEGAMESETTINGS_API UUI_SimpleGameSettingCore : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void SaveSetting() {}

	UFUNCTION()
	virtual void LoadSetting() {}

protected:
	void UpdateAttributeLevel(USlider* NewSlider, UTextBlock* NewBlock);
	void UpdateAttribute(USlider* NewSlider, UTextBlock* NewBlock);
	UFUNCTION()
	virtual void ChangeValue(float InValue) {}

	virtual void BindChangedValue(USlider* NewSlider);

	UFUNCTION()
	virtual void SelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType) {}

	virtual void BindSelectionChanged(UComboBoxString* NewComboBoxString);

	void LoadSettingsLevel(USlider* InSlider, UTextBlock* NewBlock, TFunction<float()> InFunc);
	void LoadSettingsValue(USlider* InSlider, UTextBlock* NewBlock, TFunction<float()> InFunc);

	void SetSettingsLevel(USlider* InSlider, UTextBlock* NewBlock, TFunction<void(float InLevel)> InFunc);

	void SetSettingsValue(USlider* InSlider, UTextBlock* NewBlock, TFunction<void(float InVlue)> InFunc);
	void SetSettingsValue(USlider* InSlider, UTextBlock* NewBlock, TFunction<float()> InFunc);


	void LoadBoxString(UComboBoxString* InBoxString, TFunction<FString()> InFunc);
};