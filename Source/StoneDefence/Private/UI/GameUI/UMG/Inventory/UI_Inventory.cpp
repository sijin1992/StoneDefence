// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/Inventory/UI_Inventory.h"
#include "UI/GameUI/UMG/Inventory/UI_InventorySlot.h"
#include "Components/UniformGridSlot.h"
#include "Components/UniformGridPanel.h"

void UUI_Inventory::NativeConstruct()
{
	Super::NativeConstruct();

	LayoutInventorySlot(3, 7);
}

void UUI_Inventory::LayoutInventorySlot(int32 ColumnNumber, int32 RowNumber)
{
	if (InventorySlotClass)
	{
		for (int32 MyRow = 0; MyRow < RowNumber; MyRow++)
		{
			//排序物品栏
			for (int32 MyColumn = 0; MyColumn < ColumnNumber; MyColumn++)
			{
				UUI_InventorySlot* SlotWidget = CreateWidget<UUI_InventorySlot>(GetWorld(), InventorySlotClass);
				if (SlotWidget)
				{
					//需要将UMG添加进去
					UUniformGridSlot* GridSlot = SlotArrayInventroy->AddChildToUniformGrid(SlotWidget);
					if (GridSlot)
					{
						GridSlot->SetColumn(MyColumn);
						GridSlot->SetRow(MyRow);
						GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
						GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
					}

					InventorySlotArray.Add(SlotWidget);
				}
			}
		}

		//处理数据
		const TArray<const FGuid*> IDs = GetGameState()->GetBuildingTowerIDs();
		for (int32 i = 0; i < ColumnNumber * RowNumber; i++)
		{
			InventorySlotArray[i]->GUID = *IDs[i];
		}
	}
}
