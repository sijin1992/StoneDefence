// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_ArchivesSystem.h"
#include "Core/SimpleArchivesGlobalVariable.h"
#include "Archives/UI_ArchivesBar.h"
#include "UMG/Public/Components/ScrollBox.h"
#include "UMG/Public/Components/Button.h"
#include "Data/SimpleArchivesList.h"

void UUI_ArchivesSystem::NativeConstruct()
{
	Super::NativeConstruct();
	
	LoadGameButton->OnClicked.AddDynamic(this, &UUI_ArchivesSystem::LoadGame);
	SaveGameButton->OnClicked.AddDynamic(this, &UUI_ArchivesSystem::SaveGame);

	UpdateArchivesSlot();
}

void UUI_ArchivesSystem::NativeDestruct()
{
	Super::NativeDestruct();

	SimpleSlotIndex = INDEX_NONE;
}

void UUI_ArchivesSystem::UpdateArchivesSlot()
{
	if (ArchivesBarClass)
	{
		if (ISimpleArchivesInterface* MyArchive = GetCorrectArchivesInterface())
		{
			if (FSaveSlotList* InSlotList = MyArchive->GetSlotList())
			{
				for (int32 i = 0; i < InSlotList->Slots.Num(); i++)
				{
					if (UUI_ArchivesBar* UIArchivesBar = CreateWidget<UUI_ArchivesBar>(GetWorld(), ArchivesBarClass))
					{
						SaveSlotList->AddChild(UIArchivesBar);
						UIArchivesBar->SlotIndex = i;

						//绑定使用负载的反向代理
						UIArchivesBar->ReverseProxy = FSimpleDelegate::CreateUObject(this, &UUI_ArchivesSystem::CallAllCheckBox, UIArchivesBar);

						UIArchivesBar->Update();

						//还原记录的数据
						if (InSlotList->Slots[i].bSave)
						{
							ResetArchivesBar(UIArchivesBar, &InSlotList->Slots[i]);
						}
					}
				}
			}
		}
	}
}

void UUI_ArchivesSystem::LoadGame()
{
	if (ISimpleArchivesInterface* MyArchive = GetCorrectArchivesInterface())
	{
		//打开地图
		if (MyArchive->OpenLevel(SimpleSlotIndex))
		{
			//UGameplayStatics::OpenLevel();//如果是联网游戏，则可以在此通知服务器准备关卡，因为是单机游戏，所以可以放到接口去实现
		}
	}
}

void UUI_ArchivesSystem::SaveGame()
{
	if (SimpleSlotIndex != INDEX_NONE)
	{
		if (ISimpleArchivesInterface* ArchInterface = GetCorrectArchivesInterface())
		{
			if (ArchInterface->SaveGameData(SimpleSlotIndex))
			{
				if (FSaveSlot* InSlot = ArchInterface->GetSaveSlot(SimpleSlotIndex))
				{
					CallAllArchivesBarBreak([&](UUI_ArchivesBar* Temp)
						{
							if (Temp->SlotIndex == SimpleSlotIndex)
							{
								ResetArchivesBar(Temp, InSlot);

								return true;
							}
							return false;
						});
				}
			}
		}
	}
}

bool UUI_ArchivesSystem::GetArchivesBarArray(TArray<UUI_ArchivesBar*>& InArchivesBars)
{
	for (UPanelSlot *PanelSlot : SaveSlotList->GetSlots())
	{
		if (UUI_ArchivesBar* ArchivesSlot = Cast<UUI_ArchivesBar>(PanelSlot->Content))
		{
			InArchivesBars.Add(ArchivesSlot);
		}
	}

	return InArchivesBars.Num() > 0;
}

void UUI_ArchivesSystem::CallAllCheckBox(UUI_ArchivesBar* OwnerArchivesBar)
{
	CallAllArchivesBar([&](UUI_ArchivesBar* Temp)
		{
			if (Temp != OwnerArchivesBar)
			{
				Temp->SetCheckBoxState(ECheckBoxState::Unchecked);
			}
		});
}

void UUI_ArchivesSystem::CallAllArchivesBar(TFunction<void(UUI_ArchivesBar* InArchivesBar)> InArchivesBarFunc)
{
	TArray<UUI_ArchivesBar*> InArchivesBars;
	if (GetArchivesBarArray(InArchivesBars))
	{
		for (auto& Temp : InArchivesBars)
		{
			InArchivesBarFunc(Temp);
		}
	}
}

void UUI_ArchivesSystem::CallAllArchivesBarBreak(TFunction<bool(UUI_ArchivesBar* InArchivesBar)> InArchivesBarFunc)
{
	TArray<UUI_ArchivesBar*> InArchivesBars;
	if (GetArchivesBarArray(InArchivesBars))
	{
		for (auto& Temp : InArchivesBars)
		{
			if (InArchivesBarFunc(Temp))
			{
				break;
			}
		}
	}
}

void UUI_ArchivesSystem::ResetArchivesBar(UUI_ArchivesBar* InArchivesBar, const FSaveSlot* InData)
{
	if (InArchivesBar && InData)
	{
		InArchivesBar->SetSaveGameDate(InData->DateText);
		InArchivesBar->SetGameThumbnail(InData->GameThumbnail);
		InArchivesBar->SetChapterName(InData->ChapterName);
	}
}

ISimpleArchivesInterface* UUI_ArchivesSystem::GetCorrectArchivesInterface()
{
	TArray<ISimpleArchivesInterface*>& MyArchives = SimpleArchivesGlobalVariable::GetSimpleArchivesArray();
	for (const auto& Temp : MyArchives)
	{
		if (Temp->GetSimpleWorld())
		{
			return Temp;
		}
	}
	return nullptr;
}
