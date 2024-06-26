// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/UI_RucksackSystem.h"
#include "UI/GameUI/UMG/Inventory/UI_Inventory.h"

void UUI_RucksackSystem::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI_RucksackSystem::UpdateInventorySlot(const FGuid& InventorySlotGUID, bool bInCD)
{
	if (Inventory)
	{
		Inventory->UpdateInventorySlot(InventorySlotGUID, bInCD);
	}
}
