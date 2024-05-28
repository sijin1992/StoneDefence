// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/DrawText.h"
#include "UMG/Public/Components/WidgetComponent.h"
#include "UI/UI_InformationWidget.h"

// Sets default values
ADrawText::ADrawText()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	Widget->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void ADrawText::BeginPlay()
{
	Super::BeginPlay();
	
	if (CurveFloatClass.ExternalCurve)
	{
		FOnTimelineFloat TimeLineDelegate;
		FOnTimelineEvent FinishedEvent;
		TimeLineDelegate.BindUFunction(this, FName("TimeLineRangeTime"));//绑定代理
		FinishedEvent.BindUFunction(this, FName("TimeLineFinished"));//绑定事件
		TimeLine.AddInterpFloat(CurveFloatClass.ExternalCurve, TimeLineDelegate);//添加插值
		TimeLine.SetLooping(false);//不循环
		TimeLine.PlayFromStart();//播放
		TimeLine.SetTimelineFinishedFunc(FinishedEvent);//调用回调
	}
}

// Called every frame
void ADrawText::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurveFloatClass.ExternalCurve)
	{
		TimeLine.TickTimeline(DeltaTime);
	}
}

void ADrawText::TimeLineRangeTime(float value)
{
	if (Height > 0)
	{
		FVector NewHeight = FMath::Lerp(FVector::ZeroVector, FVector(0, 0, Height), value);
		Widget->AddRelativeLocation(NewHeight);//设置UMG高度
	}
}

void ADrawText::TimeLineFinished()
{
	Destroy(true);//删掉自己
}

void ADrawText::SetTextBlock(const FString& InText, const FLinearColor& Color, float Percentage)
{
	if (UUI_InformationWidget * InfoWidget = Cast<UUI_InformationWidget>(Widget->GetUserWidgetObject()))
	{
		InfoWidget->SetTextBlockStyle(InText, Color, Percentage);
	}
}

