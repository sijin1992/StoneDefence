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

	//背景音乐
	UPROPERTY(config)
	float BackgroundSound;

	//游戏音乐
	UPROPERTY(config)
	float GameSound;

	//UI音乐
	UPROPERTY(config)
	float UISound;

	//是否支持物理破碎
	UPROPERTY(config)
	bool bSupportPhysX;

public:
	USimpleGameUserSettings();

	virtual void ApplySettings(bool bCheckForCommandLineOverrides) override;

	virtual void LoadSettings(bool bForceReload = false) override;

	virtual void SaveSettings() override;

	virtual void SetToDefaults() override;

	static USimpleGameUserSettings* GetSimpleGameUserSettings();

	class UWorld* GetWorld() const;

	void SetCurrentLanguage(const FString NewLanguage);

	FString GetCurrentLanguageType();

	void SetBackgroundSound(float VolumeMultiplier);

	void SetGameSound(float VolumeMultiplier);

	void SetUISound(float VolumeMultiplier);

	float GetBackgroundSound() const;

	float GetGameSound() const;

	float GetUISound() const;

	void SaveTDIni();

	bool LoadTDIni();

	//获取所有的环境音效
	bool GetAllAmbientSound(TArray<class AAmbientSound*>& Sounds);
	//生成游戏音效(打击声音、怪物声音、角色声音等)
	bool SpawnSoundAtLocation(USoundBase* CharacterSound, const FVector& Location);
protected:

	bool GetAllSoundClass(TArray<class USoundClass*>& Sounds);
};
