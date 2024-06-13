
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
	void UpdateAttribute(USlider* NewSlider, UTextBlock* NewBlock);

	UFUNCTION()
	virtual void ChangeValue(float InValue) {}

	virtual void BindChangedValue(USlider* NewSlider);

	UFUNCTION()
	virtual void SelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType) {}

	virtual void BindSelectionChanged(UComboBoxString* NewComboBoxString);

	void SetSettingsLevel(USlider* InSlider, UTextBlock* NewBlock, int32& InLevel);
	void SetSettingsLevel(USlider* InSlider, UTextBlock* NewBlock, TFunction<void(int32 InLevel)> InFunc);

	void LoadSettingsLevel(USlider* InSlider, UTextBlock* NewBlock, TFunction<float()> InFunc);
	void LoadBoxString(UComboBoxString* InBoxString, TFunction<FString()> InFunc);
};