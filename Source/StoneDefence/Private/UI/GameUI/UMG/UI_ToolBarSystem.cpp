// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/UI_ToolBarSystem.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/ProgressBar.h"

void UUI_ToolBarSystem::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI_ToolBarSystem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	//显示倒计时
	GameCount->SetText(FText::FromString(GetCurrentCount(GetGameState()->GetGameData().GameCount)));
	//显示玩家金币
	GameGold->SetText(FText::AsNumber(GetPlayerState()->GetPlayerData().GameGold));
	//塔的死亡数
	TowersDeathNumber->SetText(FText::AsNumber(GetGameState()->GetGameData().TowersDeathNumber));
	//杀死怪物数量
	KillSoldier->SetText(FText::AsNumber(GetGameState()->GetGameData().KillMonstersNumber));
	//怪物数量百分比
	GSQProgressBar->SetPercent(GetGameState()->GetGameData().GetPerOfRemMonsters());
	//当前阶段 04/02
	GameLevelSurplusQuantity->SetText(FText(
		FText::FromString(FString::Printf(TEXT("%2d / %2d"),
			GetGameState()->GetGameData().PerNumberOfMonsters.Num(),
			GetGameState()->GetGameData().MaxStagesAreMonsters - GetGameState()->GetGameData().PerNumberOfMonsters.Num()))));
}

FString UUI_ToolBarSystem::GetCurrentCount(float NewTimeCount)
{
	const int32 NewOurTime = FMath::Max(0, FMath::TruncToInt(NewTimeCount));
	const int32 NewMinutes = NewOurTime / 60;
	const int32 NewSeconds = NewOurTime % 60;
	return FString::Printf(TEXT("%02d:%02d"), NewMinutes, NewSeconds);//00:00
}
