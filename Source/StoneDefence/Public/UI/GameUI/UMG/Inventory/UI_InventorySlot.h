// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UI_RuleOfTheWidget.h"
#include "UI_InventorySlot.generated.h"

class UImage;
class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UUI_InventorySlot : public UUI_RuleOfTheWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UImage* TowersIcon;//塔的图标

	UPROPERTY(meta = (BindWidget))
	UImage* TowersCD;//塔技能CD蒙版

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TPBNumber;//Towers Prepare Building Number准备构建的数量

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TCOCNumber;//Towers Completion Of Construction Number完成的数量

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TowersCDValue;//塔的CD

	UPROPERTY(meta = (BindWidget))
	UButton* TISButton;//Towers Inventory Slot Button

public:
	virtual void NativeConstruct();

	UFUNCTION()
	void OnClickedWidget();
};
