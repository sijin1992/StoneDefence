// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "SimpleGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEGAMESETTINGS_API USimpleGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	//是否支持物理破碎
	UPROPERTY(config)
	bool SupportPhysX;

	void SetCurrentLanguage(const FString NewLanguage);

	FString GetCurrentLanguageType();

	static USimpleGameUserSettings* GetSimpleGameUserSettings();
};
