// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/UI_MainScreen.h"
#include "Character/Core/RuleOfTheCharacter.h"
#include "UI/GameUI/UMG/Tips/UI_TowerTip.h"
#include "UMG/Public/Blueprint/WidgetLayoutLibrary.h"
#include "UMG/Public/Components/CanvasPanelSlot.h"
#include "Global/UI_Datas.h"
#include "UMG/Public/Components/Image.h"

void UUI_MainScreen::NativeConstruct()
{
	Super::NativeConstruct();
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
