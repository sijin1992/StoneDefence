// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/UI_ArchivesWidget.h"
#include "UI_ArchivesSystem.generated.h"

class UButton;
class UUI_ArchivesBar;
class UScrollBox;
/**
 * 
 */
UCLASS()
class SIMPLEARCHIVES_API UUI_ArchivesSystem : public UUI_ArchivesWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UButton* LoadGameButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SaveGameButton;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* SaveSlotList;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUI_ArchivesBar> ArchivesBarClass;

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	//读取内容
	UFUNCTION()
	void LoadGame();
	//存储
	UFUNCTION()
	void SaveGame();

	bool GetArchivesBarArray(TArray<UUI_ArchivesBar*>& InArchivesBars);

protected:

	void UpdateArchivesSlot();

	void CallAllCheckBox(UUI_ArchivesBar* OwnerArchivesBar);

	void ResetArchivesBar(UUI_ArchivesBar* InArchivesBar, const FSaveSlot* InData);

private:

	void CallAllArchivesBar(TFunction<void(UUI_ArchivesBar* InArchivesBar)>);
	
	void CallAllArchivesBarBreak(TFunction<bool(UUI_ArchivesBar* InArchivesBar)>);
};
