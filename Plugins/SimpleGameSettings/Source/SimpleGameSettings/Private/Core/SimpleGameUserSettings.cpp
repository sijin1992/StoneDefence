// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SimpleGameUserSettings.h"
#include "Internationalization/Internationalization.h"
#include "Internationalization/Culture.h"

//关闭优化
#if PLATFORM_WINDOWS
#pragma optimize("", off)
#endif

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

USimpleGameUserSettings* USimpleGameUserSettings::GetSimpleGameUserSettings()
{
	//if (GEngine)
	//{
	//	return NULL;
	//}
	//else
	//{
	//	USimpleGameUserSettings* GameUserSetting = Cast<USimpleGameUserSettings>(GEngine->GetGameUserSettings());
	//	return GameUserSetting;
	//}
	return GEngine != NULL ? (Cast<USimpleGameUserSettings>(GEngine->GetGameUserSettings())) : NULL;
}

//打开优化
#if PLATFORM_WINDOWS
#pragma optimize("", on)
#endif