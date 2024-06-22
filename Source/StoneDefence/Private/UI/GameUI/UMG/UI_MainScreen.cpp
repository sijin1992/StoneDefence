// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/UI_MainScreen.h"
#include "Character/Core/RuleOfTheCharacter.h"
#include "UI/GameUI/UMG/Tips/UI_TowerTip.h"
#include "UMG/Public/Blueprint/WidgetLayoutLibrary.h"
#include "UMG/Public/Components/CanvasPanelSlot.h"
#include "Global/UI_Datas.h"
#include "UMG/Public/Components/Image.h"
#include "UI/GameUI/UMG/UI_RucksackSystem.h"
#include "UI/GameUI/UMG/UI_PlayerSkillSystem.h"
#include "../StoneDefenceUtils.h"
#include "UI/GameUI/UMG/UI_GameMenuSystem.h"
#include "UMG/Public/Components/VerticalBox.h"

void UUI_MainScreen::NativeConstruct()
{
	Super::NativeConstruct();

	SettingsButton->OnClicked.AddDynamic(this, &UUI_MainScreen::Settings);
	//SaveGame
	{
		FOnButtonClickedEvent Delegate;
		Delegate.AddDynamic(this, &UUI_MainScreen::SaveGame);
		GameMenuSystem->BindSaveGame(Delegate);
	}
	//SaveSettings
	{
		FOnButtonClickedEvent Delegate;
		Delegate.AddDynamic(this, &UUI_MainScreen::SaveSettings);
		GameMenuSystem->BindSaveSettings(Delegate);
	}
	//ReturnGame
	{
		FOnButtonClickedEvent Delegate;
		Delegate.AddDynamic(this, &UUI_MainScreen::ReturnGame);
		GameMenuSystem->BindReturnGame(Delegate);
	}
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

void UUI_MainScreen::UpdateInventorySlot(const FGuid& InventorySlotGUID, bool bInCD)
{
	if (RucksackSystem)
	{
		RucksackSystem->UpdateInventorySlot(InventorySlotGUID, bInCD);
	}
}

void UUI_MainScreen::UpdatePlayerSkillSlot(const FGuid& PlayerSkillSlotGUID, bool bInCD)
{
	if (PlayerSkillSystem)
	{
		PlayerSkillSystem->UpdatePlayerSkillSlot(PlayerSkillSlotGUID, bInCD);
	}
}

void UUI_MainScreen::Settings()
{
	if (GameMenuSystem->GetVisibility() == ESlateVisibility::Hidden)
	{
		NewWindows->SetVisibility(ESlateVisibility::Visible);
		GameMenuSystem->SetVisibility(ESlateVisibility::Visible);
	}
	else 
	{
		NewWindows->SetVisibility(ESlateVisibility::Hidden);
		GameMenuSystem->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UUI_MainScreen::SaveGame()
{
	StoneDefenceUtils::CreateAssistWidget<UUI_MainScreen, UUI_ArchivesSystem>(this, ArchivesSystemclass, BoxList);
}

void UUI_MainScreen::SaveSettings()
{
	StoneDefenceUtils::CreateAssistWidget<UUI_MainScreen, UUI_GameSettingsSystem>(this, GameSettingsSystemclass, BoxList);
}

void UUI_MainScreen::ReturnGame()
{
	Settings();
}
