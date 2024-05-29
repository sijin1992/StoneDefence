// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/Inventory/DragDrop/UI_IconDragDrop.h"
#include "UMG/Public/Components/Image.h"

void UUI_IconDragDrop::DrawIcon(UTexture2D* InIcon)
{
	Icon->SetBrushFromTexture(InIcon);
}
