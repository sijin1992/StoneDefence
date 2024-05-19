// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TowerDefenceLevelGameMode.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API ATowerDefenceLevelGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ATowerDefenceLevelGameMode();

	virtual void BeginPlay() override;
};
