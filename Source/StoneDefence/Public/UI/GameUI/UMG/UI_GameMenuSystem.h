// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Drop/UI_NativeOnDrop.h"
#include "UMG/Public/Components/Button.h"
#include "UI_GameMenuSystem.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UUI_GameMenuSystem : public UUI_NativeOnDrop
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UButton* ReturnGameButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SaveGameButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SaveSettingsButton;

	UPROPERTY(meta = (BindWidget))
	UButton* GameQuitButton;

public:
	virtual void NativeConstruct();

	void BindSaveGame(FOnButtonClickedEvent ClickedEvent);

	void BindSaveSettings(FOnButtonClickedEvent ClickedEvent);

	void BindReturnGame(FOnButtonClickedEvent ClickedEvent);

private:
	UFUNCTION()
	void GameQuit();
};
