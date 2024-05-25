// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BTTask/BTTaskAttackTarget.h"
#include "Character/Core/RuleOfTheCharacter.h"
#include "Character/AIController/MonsterAIController.h"
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "AIModule/Classes/BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

void UBTTaskAttackTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	//解析黑板数据
	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		Blackboard_Actor.ResolveSelectedKey(*BBAsset);
	}
	else
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("Can't Initialize task:%s"), *GetName());
	}
}

EBTNodeResult::Type UBTTaskAttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);


	//获取MonsterAIController
	if (AMonsterAIController* MyAIController = Cast<AMonsterAIController>(OwnerComp.GetOwner()))
	{
		//获取黑板
		if (UBlackboardComponent* MyBlackBoard = OwnerComp.GetBlackboardComponent())
		{
			if (Blackboard_Actor.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
			{
				//获取自身Pawn
				if (ARuleOfTheCharacter* MyPawn = Cast<ARuleOfTheCharacter>(MyAIController->GetPawn()))
				{
					//获取目标塔
					if (ARuleOfTheCharacter* TargetTower = Cast<ARuleOfTheCharacter>(MyBlackBoard->GetValueAsObject(Blackboard_Actor.SelectedKeyName)))
					{
						MyPawn->bAttack = true;
						MyAIController->AttackTarget(TargetTower);
						return EBTNodeResult::Succeeded;
					}
					else
					{
						MyPawn->bAttack = false;
						return EBTNodeResult::Succeeded;
					}
				}
			}
		}
	}

	return EBTNodeResult::Failed;
}
