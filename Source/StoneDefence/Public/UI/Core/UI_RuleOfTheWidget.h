// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/GameCore/TowerDefencePlayerController.h"
#include "Core/GameCore/TowerDefencePlayerState.h"
#include "Core/GameCore/TowerDefenceGameState.h"
#include "UI_RuleOfTheWidget.generated.h"

class UWidgetAnimation;
/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UUI_RuleOfTheWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UUI_RuleOfTheWidget(const FObjectInitializer& ObjectInitializer);

	//角色ID
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI)
	FGuid GUID;

	ATowerDefenceGameState* GetGameState();
	ATowerDefencePlayerState* GetPlayerState();
	ATowerDefencePlayerController* GetPlayerController();
	//通过名字获取蓝图动画
	UWidgetAnimation* GetNameWidgetAnimation(const FString& WidgetAnimationName) const;
};
