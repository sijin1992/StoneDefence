// Fill out your copyright notice in the Description page of Project Settings.


#include "Tutoria/UI_TutoriaList.h"
#include "UMG/Public/Animation/WidgetAnimation.h"
#include "UMG/Public/Components/ScrollBox.h"

void UUI_TutoriaList::NativeConstruct()
{
	Super::NativeConstruct();
	OpenPlayListAnim = GetNameWidgetAnimation("OpenPlayList");
	ClosePlayListAnim = GetNameWidgetAnimation("ClosePlayList");
}

void UUI_TutoriaList::Add(UWidget* InWidget)
{
	if (MediaScrollList)
	{
		MediaScrollList->AddChild(InWidget);
	}
}

UWidgetAnimation* UUI_TutoriaList::GetNameWidgetAnimation(const FString& WidgetAnimationName) const
{
	if (UWidgetBlueprintGeneratedClass* WidgetBlueprintGenerated = Cast<UWidgetBlueprintGeneratedClass>(GetClass()))
	{
		TArray<UWidgetAnimation*> TArrayAnimations = WidgetBlueprintGenerated->Animations;
		UWidgetAnimation** MyTempAnimation = TArrayAnimations.FindByPredicate([&](const UWidgetAnimation* OurAnimation) {return OurAnimation->GetFName().ToString() == (WidgetAnimationName + FString("_INST")); });
		return *MyTempAnimation;
	}
	return nullptr;
}

void UUI_TutoriaList::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (OpenPlayListAnim)
	{
		PlayAnimation(OpenPlayListAnim);
	}
}

void UUI_TutoriaList::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	if (ClosePlayListAnim)
	{
		PlayAnimation(ClosePlayListAnim);
	}
}
