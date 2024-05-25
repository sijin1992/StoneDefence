// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Core/RuleOfTheAIController.h"
#include "TowerAIController.generated.h"

class ARuleOfTheCharacter;
/**
 * 
 */
UCLASS()
class STONEDEFENCE_API ATowerAIController : public ARuleOfTheAIController
{
	GENERATED_BODY()
	
public:
	ATowerAIController();

	virtual void Tick(float DeltaTime) override;

	AActor* FindTarget();

	void AttackTarget(ARuleOfTheCharacter* AttackTarget);

protected:
	//存放找到的怪物列表
	UPROPERTY()
	TArray<ARuleOfTheCharacter*> TArrayMonsters;

	//心跳诊断
	float HeartbeatDiagnosis;

	//相当于服务节点
	void BTService_FindTarget();
};
