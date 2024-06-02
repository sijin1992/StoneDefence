// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "UI_TutoriaSystem.generated.h"

class UCheckBox;
class UButton;
class USlider;
class UImage;
class UTextBlock;
class UMediaPlayer;
class UUI_TutoriaSlot;
class UScrollBox;
class UWidgetAnimation;
class UUI_TutoriaList;
class UMediaSoundComponent;
class UComboBoxString;

//视频播放类型
enum class ETutoriaType
{
	FULL_SCREEN,
	WINDOW,
	MAX
};
/**
 * 
 */
UCLASS()
class SIMPLETUTORIA_API UUI_TutoriaSystem : public UUserWidget
{
	GENERATED_BODY()

	//声音组件
	UPROPERTY()
	UMediaSoundComponent* MediaSoundComponent;

	//播放按钮
	UPROPERTY(meta = (BindWidget))
	UButton* ReplayButton;
	//敲击按钮
	UPROPERTY(meta = (BindWidget))
	UCheckBox* SuspendButton;
	//暂停按钮
	UPROPERTY(meta = (BindWidget))
	UButton* PauseButton;
	//进度条
	UPROPERTY(meta = (BindWidget))
	USlider* MovieProgress;
	//暂停图片
	UPROPERTY(meta = (BindWidget))
	UImage* PauseImage;
	//播放时间
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayTimeText;
	//播放列表
	UPROPERTY(meta = (BindWidget))
	UUI_TutoriaList* UITutoriaList;
	//全屏缩放按钮
	UPROPERTY(meta = (BindWidget))
	UCheckBox* ScreenButton;
	//倍速按钮
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ComboPlaySpeed;

public:
	UUI_TutoriaSystem(const FObjectInitializer& ObjectInitializer);
	//播放类型
	ETutoriaType TutoriaType = ETutoriaType::WINDOW;

	//载入视频播放
	UPROPERTY(EditAnywhere, Category = UI)
	UMediaPlayer* MediaPlayer;
	//视频列表子类
	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<UUI_TutoriaSlot> TutoriaSlotClass;

	void SetTutoriaType(ETutoriaType InTutoriaType);

protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//敲击函数，暂停
	UFUNCTION()
	void ClickedCheckBox(bool ClickeWidget);
	//初始化视频资源
	UFUNCTION()
	void InitMadia();
	//播放视频
	UFUNCTION()
	void ActivationMovie();
	//通过下标播放
	UFUNCTION()
	bool PlayByIndex(int32 Index);
	//通过路径播放
	UFUNCTION()
	bool PlayByPath(const FString& InPath);
	//重播
	UFUNCTION()
	void Replay();
	//暂停
	UFUNCTION()
	void Pause();
	//关闭
	UFUNCTION()
	void Close();
	//电影播放完毕
	UFUNCTION()
	void FinishPlayMovie();
	//改变进度
	UFUNCTION()
	void ChangedValue(float InValue);
	//捕获滑条开始
	UFUNCTION()
	void MouseCaptureBegin();
	//捕获滑条结束
	UFUNCTION()
	void MouseCaptureEnd();
	//全屏按钮
	UFUNCTION()
	void ClickedCheckBoxScreen(bool ClickeWidget);
	//改变播放速度
	UFUNCTION()
	void PlaySpeed(FString SelectedItem, ESelectInfo::Type SelectionType);
};
