// Fill out your copyright notice in the Description page of Project Settings.


#include "Tutoria/UI_TutoriaSlot.h"
#include "UMG/Public/Components/Button.h"
#include "GlobalTutoriaProxy.h"

UUI_TutoriaSlot::UUI_TutoriaSlot(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	Index = INDEX_NONE;
}

void UUI_TutoriaSlot::NativeConstruct()
{
	Super::NativeConstruct();

	PlayButton->OnClicked.AddDynamic(this, &UUI_TutoriaSlot::PlayByPath);
}

bool UUI_TutoriaSlot::IsIndexValid()
{
	//return Index != INDEX_NONE;
	return TutoriaPath != "";
}

void UUI_TutoriaSlot::PlayByIndex()
{
	if (SimpleTutoriaDelegate.IsBound())
	{
		if (SimpleTutoriaDelegate.Execute(Index))
		{

		}
		else
		{

		}
	}
}

void UUI_TutoriaSlot::PlayByPath()
{
	if (SimpleTutoriaPathDelegate.IsBound())
	{
		if (SimpleTutoriaPathDelegate.Execute(TutoriaPath))
		{

		}
		else
		{

		}
	}
}
