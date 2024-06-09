// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Drop/UI_NativeOnDrop.h"
#include "UI_RucksackSystem.generated.h"

class UUI_Inventory;
/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UUI_RucksackSystem : public UUI_NativeOnDrop
{
	GENERATED_BODY()

	UPROPERTY(meta =(BindWidget))
	UUI_Inventory *Inventory;

public:
	virtual void NativeConstruct();

	void UpdateInventorySlot(const FGuid& InventorySlotGUID, bool bInCD);
};
