// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/UI_Slot.h"
#include "UI_Inventory.generated.h"

class UUniformGridPanel;
class UUI_InventorySlot;
/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UUI_Inventory : public UUI_Slot
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* SlotArrayInventroy;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUI_InventorySlot> InventorySlotClass;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	UMaterialInterface* DollMaterial;

public:
	virtual void NativeConstruct();

	FBuildingTower& GetBuildingTower();

	void LayoutInventorySlot(int32 ColumnNumber, int32 RowNumber);

	void UpdateInventorySlot(const FGuid& InventorySlotGUID, bool bInCD);

	void SpawnTowersDollPressed();

	void SpawnTowersDollReleased();
private:
	TArray<UUI_InventorySlot*> InventorySlotArray;
};
