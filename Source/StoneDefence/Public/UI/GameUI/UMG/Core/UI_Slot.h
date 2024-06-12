// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UI_RuleOfTheWidget.h"
#include "UI_Slot.generated.h"

class UImage;
class UTextBlock;
class UButton;
class UTexture2D;
/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UUI_Slot : public UUI_RuleOfTheWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UImage* SlotIcon;//图标

	UPROPERTY(meta = (BindWidget))
	UImage* SlotIconCD;//CD蒙版

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SlotNumber;//次数

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SlotCDValue;//CD

	UPROPERTY(meta = (BindWidget))
	UButton* ClickButton;//敲击按钮

	UPROPERTY(EditDefaultsOnly, Category = UI)
	FName SlotMatCDName;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	FName SlotClearValueName;

	//CD动态材质
	UPROPERTY()
	class UMaterialInstanceDynamic* CDMaterialDynamic;

public:
	//绘制CD
	void DrawSlotCD(float CDValue);
	//更新信息
	void UpdateSlotInfo(float CDValue, int32 Number);

	void UpdateSlotUI(UTexture2D* Icon, int32 ConstructionNumber);
protected:

	virtual void NativeConstruct() override;


	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void OnClickedWidget() {};

	//显示数字到Text组件上
	void DisplayNumber(UTextBlock* TextNumberBlock, float TextNumber);
	void DisplayNumber(UTextBlock* TextNumberBlock, int32 TextNumber);

	virtual void ClearSlot();
};
