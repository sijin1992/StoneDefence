// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TowerDefenceSelectLevelHUD.generated.h"

class UUI_MainSelectLevel;
/**
 * 
 */
UCLASS()
class STONEDEFENCE_API ATowerDefenceSelectLevelHUD : public AHUD
{
	GENERATED_BODY()

public:
	ATowerDefenceSelectLevelHUD();
	
	virtual void BeginPlay();
private:
	TSubclassOf<UUI_MainSelectLevel> MainSelecLevelClass;
	UUI_MainSelectLevel* MainSelectLevel;
};
