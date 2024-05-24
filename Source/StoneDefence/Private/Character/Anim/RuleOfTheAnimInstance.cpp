// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Anim/RuleOfTheAnimInstance.h"
#include "Character/Core/RuleOfTheCharacter.h"

URuleOfTheAnimInstance::URuleOfTheAnimInstance()
	:bDeath(false)
	,bAttack(false)
	,Speed(0.0f)
{

}

void URuleOfTheAnimInstance::NativeInitializeAnimation()
{

}

void URuleOfTheAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	//通常用于检测BP里的变量
	ARuleOfTheCharacter* RuleOfTheCharacter = Cast<ARuleOfTheCharacter>(TryGetPawnOwner());
	if (RuleOfTheCharacter)
	{
		bAttack = RuleOfTheCharacter->bAttack;
		Speed = RuleOfTheCharacter->GetVelocity().Size();
		bDeath = RuleOfTheCharacter->IsActive();
	}
}
