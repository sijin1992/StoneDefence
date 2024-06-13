// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/UI_SimpleGameSettingCore.h"
#include "UI_GameSettingsGame.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEGAMESETTINGS_API UUI_GameSettingsGame : public UUI_SimpleGameSettingCore
{
	GENERATED_BODY()
public:
	virtual void SaveSetting() override;
	virtual void LoadSetting() override;
};
