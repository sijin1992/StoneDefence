// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SelectLevelUI/UMG/UI_MainSelectLevel.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "UI/SelectLevelUI/UMG/Button/UI_LevelButton.h"
#include "Kismet/GameplayStatics.h"

void UUI_MainSelectLevel::NativeConstruct()
{
	Super::NativeConstruct();

	InitSelectLevelButton();

	ReturnMenuButton->OnClicked.AddDynamic(this, &UUI_MainSelectLevel::ReturnMenu);
}

void UUI_MainSelectLevel::InitSelectLevelButton()
{
	if (SelectMap)
	{
		TArray<UPanelSlot*> PanelSlotArray = SelectMap->GetSlots();
		for (UPanelSlot* PanelSlotEle : PanelSlotArray)
		{
			if (UUI_LevelButton* LevelButtonArp = Cast<UUI_LevelButton>(PanelSlotEle->Content))
			{
				AllLevelButton.Add(LevelButtonArp);
			}
		}
	}
}

void UUI_MainSelectLevel::ReturnMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), "HallMap");
}
