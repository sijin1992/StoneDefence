// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSettings/UI_GameSettingsGame.h"
#include "Widget/InputKeySelectorMapping.h"
#include "GameFramework/InputSettings.h"
#include "UMG/Public/Components/HorizontalBox.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/VerticalBox.h"
#include "UMG/Public/Components/HorizontalBoxSlot.h"

void UUI_GameSettingsGame::NativeConstruct()
{
	Super::NativeConstruct();
	UpdateInputKey();
}

void UUI_GameSettingsGame::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUI_GameSettingsGame::SaveSetting()
{

}

void UUI_GameSettingsGame::LoadSetting()
{

}

void UUI_GameSettingsGame::ChangeInputKey(FInputChord SelectedKey)
{
	if (KeySelectorTemp)
	{
		TArray<FInputActionKeyMapping> OutMapping;
		UInputSettings::GetInputSettings()->GetActionMappingByName(KeySelectorTemp->ActionName, OutMapping);
		if (OutMapping.Num())
		{
			UInputSettings::GetInputSettings()->RemoveActionMapping(OutMapping[0]);
			FInputActionKeyMapping InputActionKeyMapping;
			InputActionKeyMapping.ActionName = KeySelectorTemp->ActionName;
			InputActionKeyMapping.Key = KeySelectorTemp->SelectedKey.Key;
			InputActionKeyMapping.bShift = KeySelectorTemp->SelectedKey.bShift;
			InputActionKeyMapping.bCmd = KeySelectorTemp->SelectedKey.bCmd;
			InputActionKeyMapping.bCtrl = KeySelectorTemp->SelectedKey.bCtrl;
			InputActionKeyMapping.bAlt = KeySelectorTemp->SelectedKey.bAlt;

			UInputSettings::GetInputSettings()->AddActionMapping(InputActionKeyMapping);
			UInputSettings::GetInputSettings()->SaveKeyMappings();
		}
	}
}

void UUI_GameSettingsGame::OnSelectingKeyChanged(UInputKeySelectorMapping* InInputKey)
{
	KeySelectorTemp = InInputKey;
}

void UUI_GameSettingsGame::UpdateInputKey()
{
	TArray<FName> ActionName;
	UInputSettings::GetInputSettings()->GetActionNames(ActionName);
	for (auto& Temp : ActionName)
	{
		if (UHorizontalBox* HorizontalBox = NewObject<UHorizontalBox>(this, UHorizontalBox::StaticClass()))
		{
			if (UTextBlock* NewText = NewObject<UTextBlock>(this, UTextBlock::StaticClass(), Temp, RF_Transactional))
			{
				NewText->SetText(FText::FromName(Temp));
				//HorizontalBox->AddChild(NewText);
				UHorizontalBoxSlot* BoxSlotA = Cast<UHorizontalBoxSlot>(HorizontalBox->AddChild(NewText));
				BoxSlotA->SetSize(ESlateSizeRule::Fill);

				TArray<FInputActionKeyMapping> OutMappings;
				UInputSettings::GetInputSettings()->GetActionMappingByName(Temp, OutMappings);
				if (OutMappings.Num())
				{
					if (UInputKeySelectorMapping* KeySelector = NewObject<UInputKeySelectorMapping>(this, UInputKeySelectorMapping::StaticClass(), *FGuid::NewGuid().ToString(), RF_Transactional))
					{
						KeySelector->OnKeySelected.AddDynamic(this, &UUI_GameSettingsGame::ChangeInputKey);
						KeySelector->OnIsSelectingKeyChanged.AddDynamic(this, &UUI_GameSettingsGame::OnSelectingKeyChanged);

						FInputChord InSelectedKey;
						InSelectedKey.Key = OutMappings[0].Key;
						InSelectedKey.bShift = OutMappings[0].bShift;
						InSelectedKey.bCmd = OutMappings[0].bCmd;
						InSelectedKey.bCtrl = OutMappings[0].bCtrl;
						InSelectedKey.bAlt = OutMappings[0].bAlt;
						KeySelector->ActionName = OutMappings[0].ActionName;
						KeySelector->SetSelectedKey(InSelectedKey);

						//HorizontalBox->AddChild(KeySelector);
						UHorizontalBoxSlot* BoxSlotB = Cast<UHorizontalBoxSlot>(HorizontalBox->AddChild(KeySelector));
						BoxSlotB->SetSize(ESlateSizeRule::Fill);
					}
				}

				ActionList->AddChild(HorizontalBox);
			}
		}
	}
}
