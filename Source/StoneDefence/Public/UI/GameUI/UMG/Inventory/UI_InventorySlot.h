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
	UTextBlock* TPBNumber;//Towers Prepare Building Number准备构建的数量

	//UPROPERTY(EditDefaultsOnly, Category = UI)
	//TSubclassOf<class UStoneDefenceDragDropOperation> IconDragDrop;//拖拽实例

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UUI_IconDragDrop> IconDragDropClass;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UUI_TowerTip> TowerTipClass;//Tips蓝图类

public:
	virtual void NativeConstruct();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void OnClickedWidget() override;

	void UpdateUI();

	FBuildingTower& GetBuildingTower();

	//只是显示清除
	virtual void ClearSlot() override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Tip)
	UWidget* GetTowerTip();

	//更新炮塔信息
	void UpdateTowersBuildingInfo();

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
};
