// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Core/UI_RuleOfTheWidget.h"
#include "UMG/Public/Animation/WidgetAnimation.h"

UUI_RuleOfTheWidget::UUI_RuleOfTheWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	GUID = FGuid::NewGuid();
}

ATowerDefenceGameState* UUI_RuleOfTheWidget::GetGameState()
{
	if (GetWorld())
	{
		return GetWorld()->GetGameState<ATowerDefenceGameState>();
	}
	return nullptr;
}

class ATowerDefencePlayerState* UUI_RuleOfTheWidget::GetPlayerState()
{
	if (GetPlayerController())
	{
		return GetPlayerController()->GetPlayerState<ATowerDefencePlayerState>();
	}
	return nullptr;
}

class ATowerDefencePlayerController* UUI_RuleOfTheWidget::GetPlayerController()
{
	if (GetWorld())
	{
		return GetWorld()->GetFirstPlayerController<ATowerDefencePlayerController>();
	}
	return nullptr;
}

//通过名字获取蓝图动画
UWidgetAnimation* UUI_RuleOfTheWidget::GetNameWidgetAnimation(const FString& WidgetAnimationName) const
{
	if (UWidgetBlueprintGeneratedClass* WidgetBlueprintGenerated = Cast<UWidgetBlueprintGeneratedClass>(GetClass()))
	{
		TArray<UWidgetAnimation*> TArrayAnimations = WidgetBlueprintGenerated->Animations;
		UWidgetAnimation** MyTempAnimation = TArrayAnimations.FindByPredicate([&](const UWidgetAnimation* OurAnimation) {return OurAnimation->GetFName().ToString() == (WidgetAnimationName + FString("_INST")); });
		return *MyTempAnimation;
	}
	return nullptr;
}
