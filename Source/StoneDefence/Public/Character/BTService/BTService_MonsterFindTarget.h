// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_MonsterFindTarget.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UBTService_MonsterFindTarget : public UBTService
{
	GENERATED_BODY()

public:
	//FBlackboardKeySelector 通过反射可以设置很多种类型
	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	struct FBlackboardKeySelector BlackBoardKey_Target;//搜集对象

	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	struct FBlackboardKeySelector BlackBoardKey_Distance;//距离

	/**
	* 每帧搜集对象并计算距离
	* OwnerComp:
	* NodeMemory:
	* DeltaSeconds:
	* @return:
	*/
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void OnGameplayTaskInitialized(UGameplayTask& Task) {};

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) {};

	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) {};

private:
	//弱指针，当对象被销毁时会自动置为nullptr，无需手动销毁
	//这里的弱指针继承自Object，在slate中的弱指针时C++原生的
	TWeakObjectPtr<class ARuleOfTheCharacter> Target;
};
