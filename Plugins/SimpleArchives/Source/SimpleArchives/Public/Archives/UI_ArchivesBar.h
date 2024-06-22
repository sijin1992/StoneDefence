// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "UI_ArchivesBar.generated.h"

class UImage;
class UTextBlock;
class UCheckBox;
class UButton;
/**
 * 
 */
UCLASS()
class SIMPLEARCHIVES_API UUI_ArchivesBar : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UImage* GameThumbnail;//游戏存档的缩略图

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SaveGameDateBlock;//存档日期

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ChapterNameBlock;//存档章节名

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SaveNumberBlock;//存档数字

	UPROPERTY(meta = (BindWidget))
	UCheckBox* CheckBoxButton;

	UPROPERTY(meta = (BindWidget))
	UButton* DeleteMyDataButton;

public:
	int32 SlotIndex;

	//单播代理
	FSimpleDelegate ReverseProxy;

	UUI_ArchivesBar(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void Update();

	UFUNCTION()
	void ClickedCheckBox(bool ClickedWidget);

	UFUNCTION()
	void OnClickedWidgetDelete();

	void SetCheckBoxState(ECheckBoxState InState);

	void SetGameThumbnail(UTexture2D* InImage);

	void SetChapterName(const FText& InText);

	void SetSaveGameDate(const FText& InText);

	void ClearSlotData();
};
