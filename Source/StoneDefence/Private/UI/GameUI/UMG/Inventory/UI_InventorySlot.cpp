// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/Inventory/UI_InventorySlot.h"
#include "UMG/Public/Components/Button.h"
#include "UMG/Public/Components/Image.h"

void UUI_InventorySlot::NativeConstruct()
{
	Super::NativeConstruct();

	TISButton->OnClicked.AddDynamic(this, &UUI_InventorySlot::OnClickedWidget);
}

void UUI_InventorySlot::OnClickedWidget()
{

}

void UUI_InventorySlot::UpdateUI()
{
	if (GetBuildingTower().Icon)
	{
		TowersIcon->SetBrushFromTexture(GetBuildingTower().Icon);
		TowersIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{ 
		TowersIcon->SetVisibility(ESlateVisibility::Hidden);
	}
	//TowersIcon->SetBrushFromSoftTexture(GetBuildingTower().Icon);
}

FBuildingTower& UUI_InventorySlot::GetBuildingTower()
{
	return GetGameState()->GetBuildingTower(GUID);
}
