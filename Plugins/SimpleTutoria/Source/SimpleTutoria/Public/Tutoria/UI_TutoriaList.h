// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_TutoriaList.generated.h"

class UWidgetAnimation;
class UScrollBox;
/**
 * 
 */
UCLASS()
class SIMPLETUTORIA_API UUI_TutoriaList : public UUserWidget
{
	GENERATED_BODY()

	//媒体列表
	UPROPERTY(meta = (BindWidget))
	UScrollBox* MediaScrollList;

public:
	void Add(UWidget* InWidget);

protected:
	void NativeConstruct() override;

	//打开播放列表的蓝图动画
	UPROPERTY()
	UWidgetAnimation* OpenPlayListAnim;
	//关闭播放列表的蓝图动画
	UPROPERTY()
	UWidgetAnimation* ClosePlayListAnim;
	//通过名字获取Animation
	UWidgetAnimation* GetNameWidgetAnimation(const FString& WidgetAnimationName) const;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
};
