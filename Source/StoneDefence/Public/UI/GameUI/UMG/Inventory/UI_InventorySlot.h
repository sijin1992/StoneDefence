// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/UI_Slot.h"
#include "UI_InventorySlot.generated.h"

class UImage;
class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UUI_InventorySlot : public UUI_Slot
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UImage* TowersIcon;//塔的图标

	UPROPERTY(meta = (BindWidget))
	UImage* TowersCDMask;//塔技能CD蒙版

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TPBNumber;//Towers Prepare Building Number准备构建的数量

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TCOCNumber;//Towers Completion Of Construction Number完成的数量

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TowersCDValue;//塔的CD

	UPROPERTY(meta = (BindWidget))
	UButton* TISButton;//Towers Inventory Slot Button

	UPROPERTY(EditDefaultsOnly, Category = UI)
	FName TowersMatCDName;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	FName TowersClearValueName;

	//CD动态材质
	UPROPERTY()
	class UMaterialInstanceDynamic* CDMaterialDynamic;

public:
	virtual void NativeConstruct();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void OnClickedWidget();

	void UpdateUI();

	FBuildingTower& GetBuildingTower();

private:
	void UpdateTowerCD(float InDeltaTime);

	void DrawTowersCD(float InTowersCD);

	void DisplayNumber(UTextBlock* TextNumberBlock, int32 TextNumber);

	void UpdateTowersBuildingInfo();
};
