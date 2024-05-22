// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HallUI/UMG/UI_MainHall.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "UI/HallUI/UMG/UI_HallMenuSystem.h"
#include "UI_ArchivesSystem.h"
#include "UI_GameSettingsSystem.h"
#include "UI_TutoriaSystem.h"

//切换界面的模板
template<class T, class UserObject>
UserObject* CreateAssistWidget(T* ThisClass, UClass* AssistClass, USizeBox* WidgetArray)
{
	UserObject* UserObjectElement = nullptr;

	//播放动画的判断
	if (0)
	{
		//播放淡入
	}

	if (WidgetArray->GetChildAt(0))
	{
		if (WidgetArray->GetChildAt(0)->IsA(AssistClass))
		{
			//关闭Border淡出

			return UserObjectElement;
		}
		else
		{
			WidgetArray->ClearChildren();
		}
	}

	UserObjectElement = CreateWidget<UserObject>(ThisClass->GetWorld(), AssistClass);
	if (UserObjectElement)
	{
		if (WidgetArray->AddChild(UserObjectElement))
		{
		}
		else
		{
			UserObjectElement->RemoveFromParent();
		}
	}

	return UserObjectElement;
}

void UUI_MainHall::NativeConstruct()
{
	Super::NativeConstruct();

	if (HallMenuSystem)
	{
		//GameStart
		{
			FOnButtonClickedEvent Delegate;
			Delegate.AddDynamic(this, &UUI_MainHall::GameStart);
			HallMenuSystem->BindGameStart(Delegate);
		}
		//SecreTerritory
		{
			FOnButtonClickedEvent Delegate;
			Delegate.AddDynamic(this, &UUI_MainHall::SecreTerritory);
			HallMenuSystem->BindSecreTerritory(Delegate);
		}
		//History
		{
			FOnButtonClickedEvent Delegate;
			Delegate.AddDynamic(this, &UUI_MainHall::History);
			HallMenuSystem->BindHistory(Delegate);
		}
		//GameSettings
		{
			FOnButtonClickedEvent Delegate;
			Delegate.AddDynamic(this, &UUI_MainHall::GameSettings);
			HallMenuSystem->BindGameSettings(Delegate);
		}
		//TutorialWebsite
		{
			FOnButtonClickedEvent Delegate;
			Delegate.AddDynamic(this, &UUI_MainHall::TutorialWebsite);
			HallMenuSystem->BindTutorialWebsite(Delegate);
		}
		//Browser
		{
			FOnButtonClickedEvent Delegate;
			Delegate.AddDynamic(this, &UUI_MainHall::Browser);
			HallMenuSystem->BindBrowser(Delegate);
		}
		//SpecialContent
		{
			FOnButtonClickedEvent Delegate;
			Delegate.AddDynamic(this, &UUI_MainHall::SpecialContent);
			HallMenuSystem->BindSpecialContent(Delegate);
		}
		//QuitGame
		{
			FOnButtonClickedEvent Delegate;
			Delegate.AddDynamic(this, &UUI_MainHall::QuitGame);
			HallMenuSystem->BindQuitGame(Delegate);
		}
	}
}


void UUI_MainHall::GameStart()
{
	UGameplayStatics::OpenLevel(GetWorld(), "SelectLevel");
}

void UUI_MainHall::SecreTerritory()
{

}

void UUI_MainHall::History()
{
	CreateAssistWidget<UUI_MainHall, UUI_ArchivesSystem>(this, ArchivesSystemclass, BoxList);
}

void UUI_MainHall::GameSettings()
{
	CreateAssistWidget<UUI_MainHall, UUI_GameSettingsSystem>(this, GameSettingsSystemclass, BoxList);
}

void UUI_MainHall::TutorialWebsite()
{
	CreateAssistWidget<UUI_MainHall, UUI_TutoriaSystem>(this, TutoriaSystemclass, BoxList);
}

void UUI_MainHall::Browser()
{

}

void UUI_MainHall::SpecialContent()
{

}

void UUI_MainHall::QuitGame()
{

}

