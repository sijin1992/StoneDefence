// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskAttackTarget.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UBTTaskAttackTarget : public UBTTaskNode
{
	GENERATED_BODY()
public:

	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) {}

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) {}

	virtual void OnGameplayTaskInitialized(UGameplayTask& Task) {}

	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector Blackboard_Actor;

	//没有这个接口时，获取的数据可能不对
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	//执行任务:攻击
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
