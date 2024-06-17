// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpleGameSettingsEditor.h"
#include "ISettingsModule.h"
#include "Core/SimpleGameExtraSettings.h"
#include "Core/SimpleGameUserSettings.h"
#include "Misc/FileHelper.h"

#define LOCTEXT_NAMESPACE "FSimpleGameSettingsEditorModule"

void FSimpleGameSettingsEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingModule->RegisterSettings(
			"Project",
			"Plugins",
			"SimpleGameExtraSettings",
			LOCTEXT("SimpleGameExtraSettings", "Simple Game Extra Settings Page"),
			LOCTEXT("SimpleGameExtraSettingsDescription", "Simple Game Extra Settings Page Description"),
			GetMutableDefault<USimpleGameExtraSettings>()
		);
	}
	//自动替换自定义的GameUserSettings
	ReplaceGameUserSettingsConfig();
}

void FSimpleGameSettingsEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

void FSimpleGameSettingsEditorModule::ReplaceGameUserSettingsConfig()
{
	//将配置文件DefaultEngine里每一行的配置读取出来
	TArray<FString> Result;
	FString ConfigPath = FPaths::ProjectConfigDir() / TEXT("DefaultEngine.ini");
	FFileHelper::LoadFileToStringArray(Result, *ConfigPath);
	//判断是否有标签
	auto IsHasScriptEngineLabel = [&](const FString& InString)->int32
		{
			for (int32 i = 0; i < Result.Num(); ++i)
			{
				if (Result[i].Contains(InString))
				{
					return i;
				}
			}
			return INDEX_NONE;
		};
	//添加新的一行标签和配置
	auto AddClass = [&](FString& GameUserSettingsClassName)
		{
			FString GameUserSettingsClassNameContent;
			//如果有默认的
			if (GetDefault<USimpleGameExtraSettings>()->GameUserSettingsClass.Get())
			{
				GameUserSettingsClassNameContent = FString(FApp::GetProjectName()) + TEXT(".") + GetDefault<USimpleGameExtraSettings>()->GameUserSettingsClass.Get()->GetName();
			}
			else
			{
				GameUserSettingsClassNameContent = TEXT("SimpleGameSettings.SimpleGameUserSettings");
			}
			GameUserSettingsClassName = TEXT("GameUserSettingsClassName=/Script") / GameUserSettingsClassNameContent;
		};

	bool bSave = false;//是否需要保存

	int32 ScriptIndex = IsHasScriptEngineLabel(TEXT("[/Script/Engine.Engine]"));//获取[/Script/Engine.Engine]标签的位置
	//如果已经有了[/Script/Engine.Engine]标签
	if (ScriptIndex != INDEX_NONE)
	{
		if (IsHasScriptEngineLabel(TEXT("GameUserSettingsClassName=")) != INDEX_NONE)
		{
			int32 Index = IsHasScriptEngineLabel(TEXT("GameUserSettingsClassName=/Script/Engine.GameUserSettings"));
			//如果已经有同名配置了，直接替换类名
			if (Index != INDEX_NONE)
			{
				AddClass(Result[Index]);
				bSave = true;
			}
			//没找到暂时就不管
		}
		else
		{
			Result.Insert(FString(), ScriptIndex + 1);
			AddClass(Result[ScriptIndex + 1]);
			bSave = true;
		}
	}
	else
	{
		//如果没有[/Script/Engine.Engine]标签，就插入一个
		int32 Pos = Result.Add(TEXT("[/Script/Engine.Engine]"));
		Result.Insert(FString(), Pos + 1);
		AddClass(Result[Pos + 1]);
		bSave = true;
	}
	//保存
	if (bSave)
	{
		FFileHelper::SaveStringArrayToFile(Result, *ConfigPath);
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSimpleGameSettingsEditorModule, SimpleGameSettingsEditor)