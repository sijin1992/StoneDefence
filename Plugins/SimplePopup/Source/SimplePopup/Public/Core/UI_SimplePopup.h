// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SimplePopupType.h"
#include "UI_SimplePopup.generated.h"

class UMultiLineEditableTextBox;
class UHorizontalBox;
/**
 * 
 */
UCLASS()
class UUI_SimplePopup : public UUserWidget
{
	GENERATED_BODY()

	friend struct FPopupHandle;

	UPROPERTY(meta = (BindWidget))
	UMultiLineEditableTextBox* IntroductionBox;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* ButtonListBox;

public:

	UPROPERTY(BlueprintReadOnly, Category = "Popup")
	ESimplePopupType SimplePopupType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Popup")
	float Delay;

	//单播不支持标签
	UPROPERTY()
	FSimpleBlueprintDelegate SimpleBlueprintSureDelegate;
	FSimpleDelegate SimpleSureDelegate;

	UPROPERTY()
	FSimpleBlueprintDelegate SimpleBlueprintCancelDelegate;
	FSimpleDelegate SimpleCancelDelegate;

	UUI_SimplePopup(const FObjectInitializer& ObjectInitializer);

	void InitPopup();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetText(const FText& InNewText);

protected:
	UFUNCTION()
	void Cancel();

	UFUNCTION()
	void Sure();

};
