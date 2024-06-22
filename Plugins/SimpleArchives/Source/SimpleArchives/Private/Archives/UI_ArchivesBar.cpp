// Fill out your copyright notice in the Description page of Project Settings.


#include "Archives/UI_ArchivesBar.h"
#include "UMG/Public/Components/Button.h"
#include "UMG/Public/Components/CheckBox.h"
#include "Core/SimpleArchivesGlobalVariable.h"
#include "UMG/Public/Components/TextBlock.h"


#define LOCTEXT_NAMESPACE "ArchivesBar"

UUI_ArchivesBar::UUI_ArchivesBar(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer),
	SlotIndex(INDEX_NONE)
{

}

void UUI_ArchivesBar::NativeConstruct()
{
	Super::NativeConstruct();
	
	CheckBoxButton->OnCheckStateChanged.AddDynamic(this, &UUI_ArchivesBar::ClickedCheckBox);
	DeleteMyDataButton->OnClicked.AddDynamic(this, &UUI_ArchivesBar::OnClickedWidgetDelete);
}

void UUI_ArchivesBar::Update()
{
	SaveNumberBlock->SetText(FText::Format(LOCTEXT("SaveNumberBlock", "Current data {0}"), SlotIndex));
}

void UUI_ArchivesBar::ClickedCheckBox(bool ClickedWidget)
{
	if (CheckBoxButton->GetCheckedState() == ECheckBoxState::Checked)
	{
		SimpleSlotIndex = SlotIndex;
	}
	else
	{
		SimpleSlotIndex = INDEX_NONE;
	}
	//通知单播代理
	ReverseProxy.ExecuteIfBound();
}

void UUI_ArchivesBar::OnClickedWidgetDelete()
{

}

void UUI_ArchivesBar::SetCheckBoxState(ECheckBoxState InState)
{
	CheckBoxButton->SetCheckedState(InState);
}

void UUI_ArchivesBar::SetGameThumbnail(UTexture2D* InImage)
{

}

void UUI_ArchivesBar::SetChapterName(const FText& InText)
{
	ChapterNameBlock->SetText(InText);
}

void UUI_ArchivesBar::SetSaveGameDate(const FText& InText)
{
	SaveGameDateBlock->SetText(InText);
}

void UUI_ArchivesBar::ClearSlotData()
{
	SetSaveGameDate(FText::GetEmpty());
	SetChapterName(FText::GetEmpty());
	SetGameThumbnail(nullptr);
}

#undef LOCTEXT_NAMESPACE
