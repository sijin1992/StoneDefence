// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FSimpleScreenLoadingModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	//安装Loading
	SIMPLESCREENLOADING_API void SetupScreenLoading();

private:
	void BeginLoadingScreen(const FString &MapName);
	void EndLoadingScreen(UWorld* World);
};
