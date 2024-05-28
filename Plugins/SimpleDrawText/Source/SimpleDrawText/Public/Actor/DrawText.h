// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Curves/CurveFloat.h"
#include "Components/TimelineComponent.h"
#include "DrawText.generated.h"

UCLASS()
class SIMPLEDRAWTEXT_API ADrawText : public AActor
{
	GENERATED_BODY()
	//曲线
	UPROPERTY(EditDefaultsOnly, Category = "TimeLine")
	FRuntimeFloatCurve CurveFloatClass;

	//字体UMG
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttrubute", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* Widget;
	//获取字体高度
	UPROPERTY(EditDefaultsOnly, Category = "DrawText")
	float Height;
	
public:	
	// Sets default values for this actor's properties
	ADrawText();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


protected:
	FTimeline TimeLine;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void TimeLineRangeTime(float value);

	UFUNCTION()
	void TimeLineFinished();

	void SetTextBlock(const FString& InText, const FLinearColor& Color, float Percentage);

};
