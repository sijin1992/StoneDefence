// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_TutoriaSystem.h"
#include "Tutoria/UI_TutoriaSlot.h"
#include "MediaAssets/Public/MediaPlayer.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/CheckBox.h"
#include "UMG/Public/Components/Slider.h"
#include "UMG/Public/Components/Button.h"
#include "UMG/Public/Components/TextBlock.h"
#include "HAL/FileManager.h"
#include "MediaAssets/Public/MediaSource.h"
#include "MediaAssets/Public/MediaPlaylist.h"
#include "GlobalTutoriaProxy.h"
#include "Tutoria/UI_TutoriaList.h"
#include "MediaAssets/Public/MediaSoundComponent.h"
#include "UMG/Public/Components/ComboBoxString.h"

//关闭优化
#if PLATFORM_WINDOWS
#pragma optimize("", off)
#endif

UUI_TutoriaSystem::UUI_TutoriaSystem(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UUI_TutoriaSystem::NativeConstruct()
{
	Super::NativeConstruct();

	//SetTutoriaType(ETutoriaType::WINDOW);

	InitMadia();
	//插件全局代理
	//SimpleTutoriaDelegate.BindUObject(this, &UUI_TutoriaSystem::Play);
	SimpleTutoriaPathDelegate.BindUObject(this, &UUI_TutoriaSystem::PlayByPath);
	
	//按钮代理绑定
	ReplayButton->OnClicked.AddDynamic(this, &UUI_TutoriaSystem::Replay);
	PauseButton->OnClicked.AddDynamic(this, &UUI_TutoriaSystem::Pause);

	//绑定滑条代理
	MovieProgress->OnMouseCaptureBegin.AddDynamic(this, &UUI_TutoriaSystem::MouseCaptureBegin);
	MovieProgress->OnMouseCaptureEnd.AddDynamic(this, &UUI_TutoriaSystem::MouseCaptureEnd);
	MovieProgress->OnValueChanged.AddDynamic(this, &UUI_TutoriaSystem::ChangedValue);

	//播放结束通知
	MediaPlayer->OnEndReached.AddDynamic(this, &UUI_TutoriaSystem::FinishPlayMovie);

	//敲击按钮绑定
	SuspendButton->OnCheckStateChanged.AddDynamic(this, &UUI_TutoriaSystem::ClickedCheckBox);
	//全屏按钮绑定
	ScreenButton->OnCheckStateChanged.AddDynamic(this, &UUI_TutoriaSystem::ClickedCheckBoxScreen);
	//倍速按钮绑定
	ComboPlaySpeed->OnSelectionChanged.AddDynamic(this, &UUI_TutoriaSystem::PlaySpeed);
}

void UUI_TutoriaSystem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (MediaPlayer)
	{
		//播放时间
		float NewValue = MediaPlayer->GetTime().GetTotalSeconds() / MediaPlayer->GetDuration().GetTotalSeconds();
		MovieProgress->SetValue(NewValue);
		PlayTimeText->SetText(FText::FromString(FString::Printf(TEXT("%i / %i"), MediaPlayer->GetTime().GetMinutes(), MediaPlayer->GetDuration().GetMinutes())));
	}
}

void UUI_TutoriaSystem::SetTutoriaType(ETutoriaType InTutoriaType)
{
	TutoriaType = InTutoriaType;
	switch (InTutoriaType)
	{
		case ETutoriaType::FULL_SCREEN:
			ScreenButton->SetCheckedState(ECheckBoxState::Checked);
			break;
		case ETutoriaType::WINDOW:
			ScreenButton->SetCheckedState(ECheckBoxState::Unchecked);
			break;
	}
}

void UUI_TutoriaSystem::ClickedCheckBox(bool ClickeWidget)
{
	ActivationMovie();
}

void UUI_TutoriaSystem::InitMadia()
{
	if (MediaPlayer)
	{
		//初始化声音组件
		if (!MediaSoundComponent)
		{
			MediaSoundComponent = NewObject<UMediaSoundComponent>(this, UMediaSoundComponent::StaticClass());
			MediaSoundComponent->RegisterComponentWithWorld(GetWorld());//注册组件
			MediaSoundComponent->SetMediaPlayer(MediaPlayer);//设置媒体声音
		}

		//初始化视频组件
		TArray<FString> MediaFileNames;
		FString MediaPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / TEXT("Media/"));
		IFileManager::Get().FindFilesRecursive(MediaFileNames, *MediaPath, TEXT("*.*"), true, false);

		for (int32 i = 0; i < MediaFileNames.Num(); i++)
		{
			if (UUI_TutoriaSlot* TutoriaSlot = CreateWidget<UUI_TutoriaSlot>(GetWorld(), TutoriaSlotClass))
			{
				//TutoriaSlot->Index = i;
				//MediaPlayer->GetPlaylist()->AddFile(MediaFileNames[i]);
				TutoriaSlot->TutoriaPath = MediaFileNames[i];
				UITutoriaList->Add(TutoriaSlot);
			}
		}

		if (MediaFileNames.Num() > 0)
		{
			PlayByPath(MediaFileNames[0]);
		}
	}
/*	if (UMediaSource* Media = MediaPlayer->GetPlaylist()->Get(0))
	{
		MediaPlayer->OpenSource(Media);
	}*/	
}

void UUI_TutoriaSystem::ActivationMovie()
{
	if (MediaPlayer->IsPlaying())
	{
		MediaPlayer->Pause();
		PauseImage->SetVisibility(ESlateVisibility::HitTestInvisible);
		SuspendButton->SetCheckedState(ECheckBoxState::Unchecked);
	}
	else
	{
		MediaPlayer->Play();
		PauseImage->SetVisibility(ESlateVisibility::Hidden);
		SuspendButton->SetCheckedState(ECheckBoxState::Checked);
	}
}

bool UUI_TutoriaSystem::PlayByIndex(int32 Index)
{
	if (UMediaSource* Media = MediaPlayer->GetPlaylist()->Get(Index))
	{
		return MediaPlayer->OpenSource(Media);
	}
	return false;
}

bool UUI_TutoriaSystem::PlayByPath(const FString& InPath)
{
	return MediaPlayer->OpenFile(InPath);
}

void UUI_TutoriaSystem::Replay()
{
	if (MediaPlayer->IsReady())
	{
		MediaPlayer->Rewind();
	}
}

void UUI_TutoriaSystem::Pause()
{
	ActivationMovie();
}

void UUI_TutoriaSystem::Close()
{
	if (MediaPlayer->IsPlaying())
	{
		MediaPlayer->Pause();
		PauseImage->SetVisibility(ESlateVisibility::HitTestInvisible);
		SuspendButton->SetCheckedState(ECheckBoxState::Unchecked);
	}
	MediaPlayer->Close();
}

void UUI_TutoriaSystem::FinishPlayMovie()
{
	if (MediaPlayer->IsReady())
	{
		MediaPlayer->Close();
	}
}

void UUI_TutoriaSystem::ChangedValue(float InValue)
{
	if (MediaPlayer)
	{
		MediaPlayer->Seek(FTimespan(MediaPlayer->GetDuration().GetTicks() * InValue));
	}
}

void UUI_TutoriaSystem::MouseCaptureBegin()
{

}

void UUI_TutoriaSystem::MouseCaptureEnd()
{

}

void UUI_TutoriaSystem::ClickedCheckBoxScreen(bool ClickeWidget)
{
	if (TutoriaType == ETutoriaType::FULL_SCREEN)
	{
		RemoveFromParent();
	}
	else if(TutoriaType == ETutoriaType::WINDOW)
	{
		//WidgetGeneratedByClass会生出带有蓝图信息的实例对象
		if (UUI_TutoriaSystem* TutoriaSystem = CreateWidget<UUI_TutoriaSystem>(GetWorld(), GetClass()))
		{
			TutoriaSystem->AddToViewport(99);
			TutoriaSystem->SetTutoriaType(ETutoriaType::FULL_SCREEN);
		}
	}
}

void UUI_TutoriaSystem::PlaySpeed(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	SelectedItem.RemoveFromEnd("x");
	float NewPlaySpeed = FCString::Atof(*SelectedItem);

	if (MediaPlayer)
	{
		MediaPlayer->SetRate(NewPlaySpeed);
	}
}

//打开优化
#if PLATFORM_WINDOWS
#pragma optimize("", on)
#endif