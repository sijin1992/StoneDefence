// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/Inventory/UI_Inventory.h"
#include "UI/GameUI/UMG/Inventory/UI_InventorySlot.h"
#include "Components/UniformGridSlot.h"
#include "Components/UniformGridPanel.h"
#include "Core/GameCore/TowerDefencePlayerController.h"
#include "UI/Core/UI_Datas.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"

void UUI_Inventory::NativeConstruct()
{
	Super::NativeConstruct();

	LayoutInventorySlot(3, 7);

	GetPlayerController()->EventMouseMiddlePressed.BindUObject(this, &UUI_Inventory::SpawnTowersDollPressed);
	GetPlayerController()->EventMouseMiddleReleased.BindUObject(this, &UUI_Inventory::SpawnTowersDollReleased);
}

FBuildingTower& UUI_Inventory::GetBuildingTower()
{
	return GetGameState()->GetBuildingTower(TowerIconGUID);
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

		//处理炮塔数据
		//拿到所有塔的ID
		const TArray<const FGuid*> IDs = GetGameState()->GetBuildingTowerIDs();
		for (int32 i = 0; i < ColumnNumber * RowNumber; i++)
		{
			InventorySlotArray[i]->GUID = *IDs[i];
		}
		//拿到塔数据
		TArray<const FCharacterData*> Datas;
		if (GetGameState()->GetTowerDataFromTable(Datas))
		{
			//默认把塔都送给玩家,实际项目中会根据玩家解锁的塔生成
			for (int32 i = 0; i < Datas.Num(); i++)
			{
				InventorySlotArray[i]->GetBuildingTower().TowerID = Datas[i]->ID;
				InventorySlotArray[i]->GetBuildingTower().NeedGold = Datas[i]->Gold;
				InventorySlotArray[i]->GetBuildingTower().MaxConstructionTowersCD = Datas[i]->ConstructionTime;
				InventorySlotArray[i]->GetBuildingTower().Icon = Datas[i]->Icon.LoadSynchronous();

				InventorySlotArray[i]->UpdateUI();
			}
		}
	}
}

void UUI_Inventory::SpawnTowersDollPressed()
{
	if (GetBuildingTower().IsValid())
	{
		bLockGUID = true;
		if (GetBuildingTower().TowersConstructionNumber >= 1)//只有建造数量大于等于1才可以生成炮塔
		{
			int32 TowersID = GetBuildingTower().TowerID;
			//生成绿色模型
			if (AStaticMeshActor* MeshActor = GetGameState()->SpawnTowersDoll(TowersID))
			{
				//替换所有材质为绿色
				for (int32 i = 0; i < MeshActor->GetStaticMeshComponent()->GetNumMaterials(); i++)
				{
					MeshActor->GetStaticMeshComponent()->SetMaterial(i, DollMaterial);
				}
				TowerDoll = MeshActor;
			}
		}
	}
}

void UUI_Inventory::SpawnTowersDollReleased()
{
	if (GetBuildingTower().IsValid())//释放鼠标中键后生成绿色模型
	{
		if (TowerDoll)
		{
			if (GetBuildingTower().TowersConstructionNumber >= 1)
			{
				if (AActor* CharacterActor = GetGameState()->SpawnTower(GetBuildingTower().TowerID, 1, TowerDoll->GetActorLocation(), TowerDoll->GetActorRotation()))
				{
					GetBuildingTower().TowersConstructionNumber--;
				}
			}
			TowerDoll->Destroy();
			TowerDoll = nullptr;
		}
	}
	bLockGUID = false;
	TowerIconGUID = FGuid();//置空GUID
}
