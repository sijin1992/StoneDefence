// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RuleOfTheAIController.generated.h"

class ARuleOfTheCharacter;
/**
 * 
 */
UCLASS()
class STONEDEFENCE_API ARuleOfTheAIController : public AAIController
{
	GENERATED_BODY()

public:
	UFUNCTION(Blueprintable, BlueprintPure, Category = "Towers|Test")
	ARuleOfTheCharacter* GetTarget();

	//弱指针，当对象被销毁时会自动置为nullptr，无需手动销毁
	//这里的弱指针继承自Object，在slate中的弱指针时C++原生的
	TWeakObjectPtr<class ARuleOfTheCharacter> Target;

	virtual AActor* FindTarget() { return nullptr; };

	virtual void AttackTarget(ARuleOfTheCharacter* AttackTarget) {};
};
