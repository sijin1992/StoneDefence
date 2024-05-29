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

	//UPROPERTY(EditDefaultsOnly, Category = UI)
	//TSubclassOf<class UStoneDefenceDragDropOperation> IconDragDrop;//拖拽实例

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UUI_IconDragDrop> IconDragDropClass;

	//CD动态材质
	UPROPERTY()
	class UMaterialInstanceDynamic* CDMaterialDynamic;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UUI_TowerTip> TowerTipClass;//Tips蓝图类

public:
	virtual void NativeConstruct();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void OnClickedWidget();

	void UpdateUI();

	FBuildingTower& GetBuildingTower();

	//只是显示清除
	void ClearSlot();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Tip)
	UWidget* GetTowerTip();

protected:
	//点击时触发
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//拖拽时触发
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	//拖拽松手时触发
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

private:
	//更新炮塔CD
	void UpdateTowerCD(float InDeltaTime);
	//绘制CD
	void DrawTowersCD(float InTowersCD);
	//显示数字到Text组件上
	void DisplayNumber(UTextBlock* TextNumberBlock, int32 TextNumber);
	//更新炮塔信息
	void UpdateTowersBuildingInfo();
};
