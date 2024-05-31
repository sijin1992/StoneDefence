// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/UI_MainScreen.h"
#include "UI/GameUI/UMG/Inventory/UI_InventorySlot.h"
#include "Character/Core/RuleOfTheCharacter.h"
#include "UI/GameUI/UMG/Tips/UI_TowerTip.h"
#include "UMG/Public/Blueprint/WidgetLayoutLibrary.h"
#include "UMG/Public/Components/CanvasPanelSlot.h"
#include "UI/Core/UI_Datas.h"
#include "UMG/Public/Components/Image.h"

void UUI_MainScreen::NativeConstruct()
{
	Super::NativeConstruct();
}

bool UUI_MainScreen::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
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

void UUI_MainScreen::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//显示角色信息
	if (ARuleOfTheCharacter* InCharacter = Cast<ARuleOfTheCharacter>(GetPlayerController()->GetHitResult().GetActor()))
	{
		const FCharacterData& CharacterData = GetGameState()->GetCharacterData(InCharacter->GUID);
		if (CharacterData.IsValid())
		{
			CharacterTip->InitTip(CharacterData);
			CharacterTip->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			if (UCanvasPanelSlot* NewPanelSlot = Cast<UCanvasPanelSlot>(CharacterTip->Slot))
			{
				FVector2D ScreenLocation = FVector2D::ZeroVector;
				//世界坐标转屏幕坐标
				UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(GetPlayerController(), GetPlayerController()->GetHitResult().Location, ScreenLocation,true);
				NewPanelSlot->SetPosition(ScreenLocation);
			}
			else
			{
				CharacterTip->SetVisibility(ESlateVisibility::Hidden);
			}
		}
		else
		{
			CharacterTip->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else
	{
		CharacterTip->SetVisibility(ESlateVisibility::Hidden);
	}

	//显示集火对象
	if (ClickedTargetMonster && ClickedTargetMonster->IsActive())
	{
		if (UCanvasPanelSlot* NewPanelSlot = Cast<UCanvasPanelSlot>(FireConcentrationPoint->Slot))
		{
			FVector2D ScreenLocation = FVector2D::ZeroVector;
			//世界坐标转屏幕坐标
			UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(GetPlayerController(), ClickedTargetMonster->GetActorLocation(), ScreenLocation,true);
			
			NewPanelSlot->SetPosition(ScreenLocation);
			FireConcentrationPoint->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
	else
	{
		ClickedTargetMonster = nullptr;
		FireConcentrationPoint->SetVisibility(ESlateVisibility::Hidden);
	}
}
