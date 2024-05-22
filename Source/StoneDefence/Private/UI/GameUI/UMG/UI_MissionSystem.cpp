// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/UI_MissionSystem.h"
#include "UMG/Public/Components/Button.h"

void UUI_MissionSystem::NativeConstruct()
{
	Super::NativeConstruct();

	ConditionButton->OnClicked.AddDynamic(this, &UUI_MissionSystem::Condition);
}

void UUI_MissionSystem::Condition()
{

}
