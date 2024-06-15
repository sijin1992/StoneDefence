// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSettings/UI_GameSettingsAudio.h"
#include "Core/SimpleGameUserSettings.h"
#include "UMG/Public/Components/Slider.h"
#include "UMG/Public/Components/TextBlock.h"
#include "SimpleGameSettingMacro.h"
#include "UMG/Public/Components/Button.h"
#include "Sound/AmbientSound.h"
#include "Components/AudioComponent.h"

//关闭优化
#if PLATFORM_WINDOWS
#pragma optimize("", off)
#endif

void UUI_GameSettingsAudio::NativeConstruct()
{
	Super::NativeConstruct();
	
	BindChangedValue(AudioQualityLevelSlider);
	BindChangedValue(BackgroundSoundSlider);
	BindChangedValue(GameSoundSlider);
	BindChangedValue(UISoundSlider);
	
	BackgroundSoundButton->OnClicked.AddDynamic(this, &UUI_GameSettingsAudio::SpawnBackgroundSoundForTest);
	GameSoundButton->OnClicked.AddDynamic(this, &UUI_GameSettingsAudio::SpawnGameSoundForTest);
	UISoundButton->OnClicked.AddDynamic(this, &UUI_GameSettingsAudio::SpawnUISoundForTest);
}

void UUI_GameSettingsAudio::SaveSetting()
{
	SetSettingsLevel(AudioQualityLevelSlider, AudioQualityLevelText,
		[](float InLevel)
		{
			USimpleGameUserSettings::GetSimpleGameUserSettings()->SetAudioQualityLevel(InLevel * SCAL_ABILITY_QUALITY_LEVEL);
		});

	SetSettingsValue(BackgroundSoundSlider, BackgroundSoundText,
		[](float InLevel)
		{
			USimpleGameUserSettings::GetSimpleGameUserSettings()->SetBackgroundSound(InLevel);
		});

	SetSettingsValue(GameSoundSlider, GameSoundText,
		[](float InLevel)
		{
			USimpleGameUserSettings::GetSimpleGameUserSettings()->SetGameSound(InLevel);
		});

	SetSettingsValue(UISoundSlider, UISoundText,
		[](float InLevel)
		{
			USimpleGameUserSettings::GetSimpleGameUserSettings()->SetUISound(InLevel);
		});
}

void UUI_GameSettingsAudio::LoadSetting()
{
	LoadSettingsLevel(AudioQualityLevelSlider, AudioQualityLevelText,
		[]() 
		{
			return USimpleGameUserSettings::GetSimpleGameUserSettings()->GetAudioQualityLevel() / SCAL_ABILITY_QUALITY_LEVEL;
		});
	LoadSettingsValue(BackgroundSoundSlider, BackgroundSoundText,
		[]()
		{
			return USimpleGameUserSettings::GetSimpleGameUserSettings()->GetBackgroundSound();
		});
	LoadSettingsValue(GameSoundSlider, GameSoundText,
		[]()
		{
			return USimpleGameUserSettings::GetSimpleGameUserSettings()->GetGameSound();
		});
	LoadSettingsValue(UISoundSlider, UISoundText,
		[]()
		{
			return USimpleGameUserSettings::GetSimpleGameUserSettings()->GetUISound();
		});
}

void UUI_GameSettingsAudio::SpawnBackgroundSoundForTest()
{
	if (BackgroundSoundTestObject)
	{
		DestroyBackgroundSound();

		//重新生成新的背景环境音效
		FActorSpawnParameters Parameters;
		Parameters.Name = TEXT("BackgroundSoundTest");

		if (AAmbientSound* NewSound = GetWorld()->SpawnActor<AAmbientSound>(AAmbientSound::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Parameters))
		{
			NewSound->GetAudioComponent()->SetSound(BackgroundSoundTestObject);
			NewSound->SetLifeSpan(BackgroundSoundTestObject->Duration);
			NewSound->Play();
		}
	}
}

void UUI_GameSettingsAudio::DestroyBackgroundSound()
{
	//拿到所有的环境音效
	TArray<AAmbientSound*> NewSounds;
	USimpleGameUserSettings::GetSimpleGameUserSettings()->GetAllAmbientSound(NewSounds);
	for (AAmbientSound* Temp : NewSounds)
	{
		//找到之前生成的旧的环境音效并删除
		if (Temp->GetName() == TEXT("BackgroundSoundTest"))
		{
			Temp->Stop();
			Temp->Destroy();
			break;
		}
	}
}

void UUI_GameSettingsAudio::SpawnGameSoundForTest()
{
	DestroyBackgroundSound();

	if (GameSoundTestObject)
	{
		USimpleGameUserSettings::GetSimpleGameUserSettings()->SpawnSoundAtLocation(GameSoundTestObject, GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());
	}
}

void UUI_GameSettingsAudio::SpawnUISoundForTest()
{
	DestroyBackgroundSound();
}

void UUI_GameSettingsAudio::ChangeValue(float InValue)
{
	UpdateAttributeLevel(AudioQualityLevelSlider, AudioQualityLevelText);

	UpdateAttribute(BackgroundSoundSlider, BackgroundSoundText);
	UpdateAttribute(GameSoundSlider, GameSoundText);
	UpdateAttribute(UISoundSlider, UISoundText);

	SaveSetting();
}

//打开优化
#if PLATFORM_WINDOWS
#pragma optimize("", on)
#endif