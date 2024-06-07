// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/Drop/UI_NativeOnDrop.h"
#include "UI/GameUI/UMG/Inventory/UI_InventorySlot.h"
#include "DragDrop/StoneDefenceDragDropOperation.h"

bool UUI_NativeOnDrop::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	bool bDrop = false;//有没有释放成功

	if (UStoneDefenceDragDropOperation* StoneDefenceDragDropOperation = Cast<UStoneDefenceDragDropOperation>(InOperation))
	{
		if (UUI_InventorySlot* MyInventorySlot = Cast<UUI_InventorySlot>(StoneDefenceDragDropOperation->Payload))
		{
			MyInventorySlot->GetBuildingTower().bDragIcon = false;
			//更新UI
			MyInventorySlot->UpdateUI();

			bDrop = true;
		}
	}
	return bDrop;
}
