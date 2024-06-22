// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HallUI/UMG/UI_MainHall.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "UI/HallUI/UMG/UI_HallMenuSystem.h"
#include "UI_ArchivesSystem.h"
#include "UI_GameSettingsSystem.h"
#include "UI_TutoriaSystem.h"
#include "../StoneDefenceUtils.h"

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
	StoneDefenceUtils::CreateAssistWidget<UUI_MainHall, UUI_ArchivesSystem>(this, ArchivesSystemclass, BoxList);
}

void UUI_MainHall::GameSettings()
{
	StoneDefenceUtils::CreateAssistWidget<UUI_MainHall, UUI_GameSettingsSystem>(this, GameSettingsSystemclass, BoxList);
}

void UUI_MainHall::TutorialWebsite()
{
	StoneDefenceUtils::CreateAssistWidget<UUI_MainHall, UUI_TutoriaSystem>(this, TutoriaSystemclass, BoxList);
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

