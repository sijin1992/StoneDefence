// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/Inventory/UI_InventorySlot.h"
#include "UMG/Public/Components/Button.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/TextBlock.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "DragDrop/StoneDefenceDragDropOperation.h"
#include "UI/GameUI/UMG/Inventory/DragDrop/UI_IconDragDrop.h"
#include "UI/GameUI/UMG/Tips/UI_TowerTip.h"

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

	if (GetBuildingTower().TowersConstructionNumber > 0)
	{
		TCOCNumber->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (GetBuildingTower().TowersPrepareBuildingNumber > 0)
	{
		TPBNumber->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	//TowersIcon->SetBrushFromSoftTexture(GetBuildingTower().Icon);
}

FBuildingTower& UUI_InventorySlot::GetBuildingTower()
{
	return GetGameState()->GetBuildingTower(GUID);
}

void UUI_InventorySlot::ClearSlot()
{
	TowersIcon->SetVisibility(ESlateVisibility::Hidden);
	TowersCDMask->SetVisibility(ESlateVisibility::Hidden);
	TPBNumber->SetVisibility(ESlateVisibility::Hidden);
	TCOCNumber->SetVisibility(ESlateVisibility::Hidden);
	TowersCDValue->SetVisibility(ESlateVisibility::Hidden);
}

UWidget* UUI_InventorySlot::GetTowerTip()
{
	if (TowerTipClass)
	{
		if (UUI_TowerTip* TowerTip = CreateWidget<UUI_TowerTip>(GetWorld(), TowerTipClass))
		{
			const FCharacterData& TowerDataInfo = GetGameState()->GetCharacterDataByID(GetBuildingTower().TowerID);
			if (TowerDataInfo.IsValid())
			{
				TowerTip->InitTip(GetGameState()->GetCharacterDataByID(GetBuildingTower().TowerID));
				return TowerTip;
			}
		}
	}
	return nullptr;
}

FReply UUI_InventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton || InMouseEvent.IsTouchEvent())
	{
		FReply Reply = FReply::Handled();
		TSharedPtr<SWidget> SlateWidgetDrag = GetCachedWidget();
		if (SlateWidgetDrag.IsValid())
		{
			Reply.DetectDrag(SlateWidgetDrag.ToSharedRef(), EKeys::RightMouseButton);//当拖拽时生成图标
			return Reply;
		}
	}

	return FReply::Handled();
}

void UUI_InventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	if (GetBuildingTower().IsValid() && IconDragDropClass)
	{
		//创建拖拽Icon的Widget
		if (UUI_IconDragDrop* IconDragDrop = CreateWidget<UUI_IconDragDrop>(GetWorld(), IconDragDropClass))
		{
			//创建拖拽实例
			if (UStoneDefenceDragDropOperation* StoneDefenceDragDropOperation = NewObject<UStoneDefenceDragDropOperation>(GetTransientPackage(), UStoneDefenceDragDropOperation::StaticClass()))
			{
				StoneDefenceDragDropOperation->SetFlags(RF_StrongRefOnFrame);//将其标签设为强引用类型，通知GC不要很快回收
				//StoneDefenceDragDropOperation->DefaultDragVisual = this;//将自身设置默认的拖拽，会生成跟自身一样大小的图标
				IconDragDrop->DrawIcon(GetBuildingTower().Icon);//绘制
				StoneDefenceDragDropOperation->DefaultDragVisual = IconDragDrop;
				StoneDefenceDragDropOperation->Payload = this;//把自己传过去，这样就可以对数据进行交换，且可以获取数据知道拖拽的是哪个
				OutOperation = StoneDefenceDragDropOperation;

				GetBuildingTower().bDragIcon = true;//将数据层是否正在被拖拽设为true

				ClearSlot();
			}
		}
	}

	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

bool UUI_InventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	bool bDrop = false;//有没有释放成功
	if (UStoneDefenceDragDropOperation* StoneDefenceDragDropOperation = Cast<UStoneDefenceDragDropOperation>(InOperation))
	{
		if (UUI_InventorySlot* MyInventorySlot = Cast<UUI_InventorySlot>(StoneDefenceDragDropOperation->Payload))
		{
			MyInventorySlot->GetBuildingTower().bDragIcon = false;
			GetGameState()->RequestInventorySlotSwap(GUID, MyInventorySlot->GUID);//交换数据
			//更新UI
			UpdateTowersBuildingInfo();
			UpdateUI();
			MyInventorySlot->UpdateTowersBuildingInfo();
			MyInventorySlot->UpdateUI();

			bDrop = true;
		}
	}
	return bDrop;
}

void UUI_InventorySlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{

}

void UUI_InventorySlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{

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