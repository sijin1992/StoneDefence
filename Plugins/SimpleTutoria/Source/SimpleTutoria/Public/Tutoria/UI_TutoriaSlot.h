// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "UI_TutoriaSlot.generated.h"

class UMediaSource;
class UButton;
/**
 * 
 */
UCLASS()
class SIMPLETUTORIA_API UUI_TutoriaSlot : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UButton* PlayButton;

public:
	UUI_TutoriaSlot(const FObjectInitializer& ObjectInitializer);

	//视频源下标
	UPROPERTY()
	int32 Index;
	//视频路径
	UPROPERTY()
	FString TutoriaPath;

	bool IsIndexValid();
	//通过下标播放
	UFUNCTION()
	void PlayByIndex();
	//通过路径播放
	UFUNCTION()
	void PlayByPath();

protected:
	virtual void NativeConstruct() override;
};
