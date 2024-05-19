// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UI_RuleOfTheWidget.h"
#include "UI_MainSelectLevel.generated.h"

class UUI_LevelButton;
class UButton;
class UCanvasPanel;
/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UUI_MainSelectLevel : public UUI_RuleOfTheWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UButton* ReturnMenuButton;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* SelectMap;

public:
	virtual void NativeConstruct();

protected:
	void InitSelectLevelButton();
	UFUNCTION()
	void ReturnMenu();

private:
	TArray<UUI_LevelButton*> AllLevelButton;
	
};
