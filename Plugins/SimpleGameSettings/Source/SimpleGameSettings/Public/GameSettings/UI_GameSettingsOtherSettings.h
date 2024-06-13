// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/UI_SimpleGameSettingCore.h"
#include "UI_GameSettingsOtherSettings.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEGAMESETTINGS_API UUI_GameSettingsOtherSettings : public UUI_SimpleGameSettingCore
{
	GENERATED_BODY()
public:
	virtual void SaveSetting() override;
	virtual void LoadSetting() override;
};
