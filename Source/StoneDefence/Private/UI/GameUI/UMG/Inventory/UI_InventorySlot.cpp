// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/Inventory/UI_InventorySlot.h"
#include "UMG/Public/Components/Button.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/TextBlock.h"
#include "Materials/MaterialInstanceDynamic.h"

void UUI_InventorySlot::NativeConstruct()
{
	Super::NativeConstruct();

	TISButton->OnClicked.AddDynamic(this, &UUI_InventorySlot::OnClickedWidget);

	if (TowersCDMask)
	{
		CDMaterialDynamic = TowersCDMask->GetDynamicMaterial();
	}
}

void UUI_InventorySlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (GetBuildingTower().IsValid())
	{
		if (!GetBuildingTower().bLockCD)
		{
			if (!GetBuildingTower().bDragIcon)
			{
				UpdateTowerCD(InDeltaTime);
			}
		}
	}
}

void UUI_InventorySlot::OnClickedWidget()
{
	if (GetBuildingTower().IsValid())
	{
		if (/*GetBuildingTower().NeedGold <= */ 1)
		{
			GetBuildingTower().TowersPrepareBuildingNumber++;
			if (GetBuildingTower().CurrentConstructionTowersCD <= 0)
			{
				GetBuildingTower().ResetCD();
			}
		}
	}
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

void UUI_InventorySlot::UpdateTowerCD(float InDeltaTime)
{
	if (GetBuildingTower().CurrentConstructionTowersCD > 0)
	{
		DrawTowersCD(GetBuildingTower().GetTowerConstructionTimePercentage());
		GetBuildingTower().CurrentConstructionTowersCD -= InDeltaTime;
		GetBuildingTower().bCallUpdateTowersInfo = true;
		UpdateTowersBuildingInfo();
	}
	else if(GetBuildingTower().bCallUpdateTowersInfo)
	{
		GetBuildingTower().bCallUpdateTowersInfo = false;
		//准备建造炮塔
		GetBuildingTower().TowersPrepareBuildingNumber--;
		GetBuildingTower().TowersConstructionNumber++;

		DrawTowersCD(0.0f);

		if (GetBuildingTower().TowersPrepareBuildingNumber > 0)
		{
			GetBuildingTower().ResetCD();
		}
		UpdateTowersBuildingInfo();
	}
}

void UUI_InventorySlot::DrawTowersCD(float InTowersCD)
{
	if (CDMaterialDynamic)
	{
		if (InTowersCD > 0.0f && InTowersCD < 1.0f)
		{
			CDMaterialDynamic->SetScalarParameterValue(TowersClearValueName, true);
			TowersCDMask->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			CDMaterialDynamic->SetScalarParameterValue(TowersClearValueName, false);
			TowersCDMask->SetVisibility(ESlateVisibility::Hidden);
		}

		CDMaterialDynamic->SetScalarParameterValue(TowersMatCDName, InTowersCD);
	}
}

void UUI_InventorySlot::DisplayNumber(UTextBlock* TextNumberBlock, int32 TextNumber)
{
	if (TextNumber < 1 || !GetBuildingTower().IsValid())
	{
		TextNumberBlock->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		TextNumberBlock->SetText(FText::FromString(FString::Printf(TEXT("%02d"), TextNumber)));
		TextNumberBlock->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UUI_InventorySlot::UpdateTowersBuildingInfo()
{
	DisplayNumber(TowersCDValue, GetBuildingTower().CurrentConstructionTowersCD);
	DisplayNumber(TCOCNumber, GetBuildingTower().TowersConstructionNumber);
	DisplayNumber(TPBNumber, GetBuildingTower().TowersPrepareBuildingNumber);
}
