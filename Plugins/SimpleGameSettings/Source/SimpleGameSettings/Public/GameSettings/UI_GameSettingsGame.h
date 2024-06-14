// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/UI_SimpleGameSettingCore.h"
#include "UI_GameSettingsGame.generated.h"

class UVerticalBox;
class UInputKeySelectorMapping;
/**
 * 
 */
UCLASS()
class SIMPLEGAMESETTINGS_API UUI_GameSettingsGame : public UUI_SimpleGameSettingCore
{
	GENERATED_BODY()

	UPROPERTY(meta = (Bindwidget))
	UVerticalBox* ActionList;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void SaveSetting() override;
	virtual void LoadSetting() override;
	
	UFUNCTION()
	void ChangeInputKey(FInputChord SelectedKey);

	UFUNCTION()
	void OnSelectingKeyChanged(UInputKeySelectorMapping* InInputKey);
protected:
	void UpdateInputKey();

	//由源码中UInputKeySelector类复制修改得来，负责过渡
	UInputKeySelectorMapping* KeySelectorTemp;
};
