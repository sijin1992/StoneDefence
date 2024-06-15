// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpleGameSettings.h"
#include "Framework/Application/SlateApplication.h"
#include "SlateSound/SimpleSlateSound.h"

#define LOCTEXT_NAMESPACE "FSimpleGameSettingsModule"

void FSimpleGameSettingsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	//获取程序应用
	FSlateApplication& CurrentSlateApp = FSlateApplication::Get();
	//注册自定义UI声音
	CurrentSlateApp.InitializeSound(TSharedRef<FSimpleSlateSoundDevice>(new FSimpleSlateSoundDevice()));
}

void FSimpleGameSettingsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSimpleGameSettingsModule, SimpleGameSettings)