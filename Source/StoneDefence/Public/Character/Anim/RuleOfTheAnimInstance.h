// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RuleOfTheAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API URuleOfTheAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttribute")
	bool bDeath;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttribute")
	bool bAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttribute")
	float Speed;

	URuleOfTheAnimInstance();

	virtual void NativeInitializeAnimation();

	virtual void NativeUpdateAnimation(float DeltaSeconds);

protected:
	bool IsDelayUpdate(float DeltaSeconds);

private:

	float CurrentTime;
	bool bDelayTime;//是否初始化成功
};
