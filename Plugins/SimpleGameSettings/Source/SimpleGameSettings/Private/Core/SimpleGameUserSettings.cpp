// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SimpleGameUserSettings.h"
#include "Internationalization/Internationalization.h"
#include "Internationalization/Culture.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/AmbientSound.h"
#include "Components/AudioComponent.h"
#include "Engine/World.h"

//关闭优化
#if PLATFORM_WINDOWS
#pragma optimize("", off)
#endif

USimpleGameUserSettings::USimpleGameUserSettings()
{
	
}

void USimpleGameUserSettings::ApplySettings(bool bCheckForCommandLineOverrides)
{
	Super::ApplySettings(bCheckForCommandLineOverrides);
	//保存配置
	SaveTDIni();
}

void USimpleGameUserSettings::LoadSettings(bool bForceReload)
{
	Super::LoadSettings(bForceReload);
	//读取配置
	LoadTDIni();
}

void USimpleGameUserSettings::SaveSettings()
{
	Super::SaveSettings();
}

void USimpleGameUserSettings::SetToDefaults()
{
	Super::SetToDefaults();

	BackgroundSound = 1.0f;

	GameSound = 1.0f;

	UISound = 1.0f;
}

USimpleGameUserSettings* USimpleGameUserSettings::GetSimpleGameUserSettings()
{
	return GEngine != NULL ? (Cast<USimpleGameUserSettings>(GEngine->GetGameUserSettings())) : NULL;
}

UWorld* USimpleGameUserSettings::GetWorld() const
{
	return GWorld;//编辑器里获取的是编辑器的World，游戏里获取的是游戏的World
}

void USimpleGameUserSettings::SetCurrentLanguage(const FString NewLanguage)
{
	if (NewLanguage == "English" || NewLanguage == "en")
	{
		FInternationalization::Get().SetCurrentCulture("en-US");
	}
	else if (NewLanguage == "Chinese" || NewLanguage == "zh" || NewLanguage == "zh-CN")
	{
		FInternationalization::Get().SetCurrentCulture("zh-Hans-CN");
	}
}

FString USimpleGameUserSettings::GetCurrentLanguageType()
{
	if (FInternationalization::Get().GetCurrentCulture().Get().GetName() == "en" ||
		FInternationalization::Get().GetCurrentCulture().Get().GetName() == "en-US")
	{
		return "English";
	}
	else if(FInternationalization::Get().GetCurrentCulture().Get().GetName() == "zh" ||
		FInternationalization::Get().GetCurrentCulture().Get().GetName() == "zh-CN" ||
		FInternationalization::Get().GetCurrentCulture().Get().GetName() == "zh-Hans-CN")
	{
		return "Chinese";
	}
	return "English";
}

void USimpleGameUserSettings::SetBackgroundSound(float VolumeMultiplier)
{
	BackgroundSound = VolumeMultiplier;

	TArray<USoundClass*> TempSound;
	GetAllSoundClass(TempSound);
	for (USoundClass* NewSoundClass : TempSound)
	{
		NewSoundClass->Properties.Volume = BackgroundSound;
	}
}

void USimpleGameUserSettings::SetGameSound(float VolumeMultiplier)
{
	GameSound = VolumeMultiplier;
}

void USimpleGameUserSettings::SetUISound(float VolumeMultiplier)
{
	UISound = VolumeMultiplier;
}

float USimpleGameUserSettings::GetBackgroundSound() const
{
	return BackgroundSound;
}

float USimpleGameUserSettings::GetGameSound() const
{
	return GameSound;
}

float USimpleGameUserSettings::GetUISound() const
{
	return UISound;
}

void USimpleGameUserSettings::SaveTDIni()
{
	const TCHAR* Section = TEXT("SimpleGameUserSettingsSound");
	GConfig->SetFloat(Section, TEXT("sg.BackgroundSound"), BackgroundSound, GGameUserSettingsIni);
	GConfig->SetFloat(Section, TEXT("sg.GameSound"), GameSound, GGameUserSettingsIni);
	GConfig->SetFloat(Section, TEXT("sg.UISound"), UISound, GGameUserSettingsIni);
	GConfig->SetBool(Section, TEXT("sg.bSupportPhysX"), bSupportPhysX, GGameUserSettingsIni);
	GConfig->SetString(Section, TEXT("sg.Language"), *GetCurrentLanguageType(), GGameUserSettingsIni);

}

bool USimpleGameUserSettings::LoadTDIni()
{
	const TCHAR* Section = TEXT("SimpleGameUserSettingsSound");
	FString LanguageString;
	bool bSuccess =
		GConfig->GetFloat(Section, TEXT("sg.BackgroundSound"), BackgroundSound, GGameUserSettingsIni) &&
		GConfig->GetFloat(Section, TEXT("sg.GameSound"), GameSound, GGameUserSettingsIni) &&
		GConfig->GetFloat(Section, TEXT("sg.UISound"), UISound, GGameUserSettingsIni) &&
		GConfig->GetBool(Section, TEXT("sg.bSupportPhysX"), bSupportPhysX, GGameUserSettingsIni) &&
		GConfig->GetString(Section, TEXT("sg.Language"), LanguageString, GGameUserSettingsIni);

	SetCurrentLanguage(LanguageString);

	return bSuccess;
}

bool USimpleGameUserSettings::GetAllAmbientSound(TArray<class AAmbientSound*>& Sounds)
{
	TArray<AActor*> ClassArrayTemp;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAmbientSound::StaticClass(), ClassArrayTemp);

	for (AActor* Temp : ClassArrayTemp)
	{
		if (AAmbientSound* AmbientSound = Cast<AAmbientSound>(Temp))
		{
			Sounds.Add(AmbientSound);
		}
	}

	return Sounds.Num() > 0;
}

bool USimpleGameUserSettings::SpawnSoundAtLocation(USoundBase* CharacterSound, const FVector& Location)
{
	if (CharacterSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), CharacterSound, Location, GameSound, 1.0f);
	}
	return CharacterSound != nullptr;
}

bool USimpleGameUserSettings::GetAllSoundClass(TArray<class USoundClass*>& Sounds)
{
	TArray<AActor*> ClassArrayTemp;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAmbientSound::StaticClass(), ClassArrayTemp);

	for (AActor* Temp : ClassArrayTemp)
	{
		AAmbientSound* AmbientSound = Cast<AAmbientSound>(Temp);
		if (AmbientSound &&
			AmbientSound->GetAudioComponent() &&
			AmbientSound->GetAudioComponent()->Sound &&
			AmbientSound->GetAudioComponent()->Sound->GetSoundClass())
		{
			Sounds.Add(AmbientSound->GetAudioComponent()->Sound->GetSoundClass());
		}
	}

	return Sounds.Num() > 0;
}

//打开优化
#if PLATFORM_WINDOWS
#pragma optimize("", on)
#endif