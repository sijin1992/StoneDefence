// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/UI_GameMenuSystem.h"

void UUI_GameMenuSystem::NativeConstruct()
{
	Super::NativeConstruct();

	GameQuitButton->OnClicked.AddDynamic(this, &UUI_GameMenuSystem::GameQuit);
}

void UUI_GameMenuSystem::BindSaveGame(FOnButtonClickedEvent ClickedEvent)
{
	SaveGameButton->OnClicked = ClickedEvent;
}

void UUI_GameMenuSystem::BindSaveSettings(FOnButtonClickedEvent ClickedEvent)
{
	SaveSettingsButton->OnClicked = ClickedEvent;
}

void UUI_GameMenuSystem::BindReturnGame(FOnButtonClickedEvent ClickedEvent)
{
	ReturnGameButton->OnClicked = ClickedEvent;
}

void UUI_GameMenuSystem::GameQuit()
{

}
