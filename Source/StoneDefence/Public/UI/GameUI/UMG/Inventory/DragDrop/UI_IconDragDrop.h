// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UI_RuleOfTheWidget.h"
#include "UI_IconDragDrop.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UUI_IconDragDrop : public UUI_RuleOfTheWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UImage* Icon;

public:
	
	void DrawIcon(UTexture2D* InIcon);
};
