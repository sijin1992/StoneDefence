// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/Core/UI_Slot.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/Button.h"
#include "Engine/Texture2D.h"
#include "Materials/MaterialInstanceDynamic.h"

void UUI_Slot::NativeConstruct()
{
	Super::NativeConstruct();

	ClickButton->OnClicked.AddDynamic(this, &UUI_Slot::OnClickedWidget);

	CDMaterialDynamic = SlotIconCD->GetDynamicMaterial();
}

void UUI_Slot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUI_Slot::UpdateSlotUI(UTexture2D* Icon, int32 Number)
{
	if (Icon)
	{
		SlotIcon->SetBrushFromTexture(Icon);
		SlotIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		SlotIcon->SetVisibility(ESlateVisibility::Hidden);
	}

	if (Number > 0)
	{
		SlotNumber->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UUI_Slot::DrawSlotCD(float CDValue)
{
	if (CDMaterialDynamic)
	{
		if (CDValue > 0.0f && CDValue < 1.0f)
		{
			CDMaterialDynamic->SetScalarParameterValue(SlotClearValueName, true);
			SlotIconCD->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			CDMaterialDynamic->SetScalarParameterValue(SlotClearValueName, false);
			SlotIconCD->SetVisibility(ESlateVisibility::Hidden);
		}

		CDMaterialDynamic->SetScalarParameterValue(SlotMatCDName, CDValue);
	}
}

void UUI_Slot::DisplayNumber(UTextBlock* TextNumberBlock, float TextNumber)
{
	if (TextNumber <= 0.0f)
	{
		TextNumberBlock->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		if (TextNumber > 0.0f && TextNumber <= 1.0f)
		{
			TextNumberBlock->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), TextNumber)));
		}
		else
		{
			TextNumberBlock->SetText(FText::FromString(FString::Printf(TEXT("%02d"), (int32)TextNumber)));
		}
		TextNumberBlock->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UUI_Slot::DisplayNumber(UTextBlock* TextNumberBlock, int32 TextNumber)
{
	if (TextNumber < 1)
	{
		TextNumberBlock->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		TextNumberBlock->SetText(FText::FromString(FString::Printf(TEXT("%02d"), TextNumber)));
		TextNumberBlock->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UUI_Slot::UpdateSlotInfo(float CDValue, int32 Number)
{
	DisplayNumber(SlotCDValue, CDValue);
	DisplayNumber(SlotNumber, Number);
}

void UUI_Slot::ClearSlot()
{
	SlotIcon->SetVisibility(ESlateVisibility::Hidden);
	SlotIconCD->SetVisibility(ESlateVisibility::Hidden);
	SlotNumber->SetVisibility(ESlateVisibility::Hidden);
	SlotCDValue->SetVisibility(ESlateVisibility::Hidden);
}
