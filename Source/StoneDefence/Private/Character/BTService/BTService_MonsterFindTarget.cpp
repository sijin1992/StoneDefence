// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BTService/BTService_MonsterFindTarget.h"
#include "Character/AIController/MonsterAIController.h"
#include "Character/Core/RuleOfTheCharacter.h"
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIModule/Classes/BehaviorTree/Blackboard/BlackboardKeyType.h"

void UBTService_MonsterFindTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	//拿到AIController
	if (ARuleOfTheAIController* CharacterAIController = Cast<ARuleOfTheAIController>(OwnerComp.GetOwner()))
	{
		//拿到保存的数据
		if (UBlackboardComponent* MyBlackBoard = OwnerComp.GetBlackboardComponent())
		{
			//通过基类方法寻找对象
			if (ARuleOfTheCharacter* NewTarget = Cast<ARuleOfTheCharacter>(CharacterAIController->FindTarget()))
			{
				//如果找到的对象与目标不一样
				if (CharacterAIController->Target != NewTarget)
				{
					//拿到当前的对象
					if (ARuleOfTheCharacter* CharacterSelf = Cast<ARuleOfTheCharacter>(CharacterAIController->GetPawn()))
					{
						//让自身对象立即停止移动
						CharacterSelf->GetCharacterMovement()->StopMovementImmediately();
					}
					//更换目标
					CharacterAIController->Target = NewTarget;
				}
				//如果对象有意义
				if (CharacterAIController->Target.IsValid())
				{
					//如果目标还活动将数据保存到黑板
					if (CharacterAIController->Target->IsActive())
					{
						//获取自身到目标塔的方向向量
						FVector NewTargetV = CharacterAIController->GetPawn()->GetActorLocation() - CharacterAIController->Target.Get()->GetActorLocation();
						NewTargetV.Normalize();
						//方向单位向量乘以攻击范围后再加上目标的位置
						FVector CurrentLocation = NewTargetV * 800.0f + CharacterAIController->Target.Get()->GetActorLocation();
						MyBlackBoard->SetValueAsVector(BlackBoardKey_TargetLocation.SelectedKeyName, CurrentLocation);

						MyBlackBoard->SetValueAsObject(BlackBoardKey_Target.SelectedKeyName, CharacterAIController->Target.Get());//为啥要用Target.Get(),因为Target时弱指针，需要转化成对象
					}
					else
					{
						//如果目标已经死亡,设置为NULL
						MyBlackBoard->SetValueAsObject(BlackBoardKey_Target.SelectedKeyName, NULL);
						MyBlackBoard->SetValueAsVector(BlackBoardKey_TargetLocation.SelectedKeyName, FVector::ZeroVector);
					}
				}
				else
				{
					//对象无意义也置为NULL
					MyBlackBoard->SetValueAsObject(BlackBoardKey_Target.SelectedKeyName, NULL);
					MyBlackBoard->SetValueAsVector(BlackBoardKey_TargetLocation.SelectedKeyName, FVector::ZeroVector);
				}
			}
			//获取距离
			if (CharacterAIController->Target.IsValid())
			{
				FVector Mylocation = CharacterAIController->GetPawn()->GetActorLocation();
				FVector TMDistance = Mylocation - CharacterAIController->Target->GetActorLocation();
				if (TMDistance.Size() > 2200)
				{
					if (ARuleOfTheCharacter* MonsterAI = Cast<ARuleOfTheCharacter>(CharacterAIController->GetPawn()))
					{
						//如果大于攻击距离就设置为不攻击
						MonsterAI->bAttack = false;
					}
				}

				MyBlackBoard->SetValueAsFloat(BlackBoardKey_Distance.SelectedKeyName, TMDistance.Size());
			}
			else
			{
				//目标没有有意义设置距离为0
				MyBlackBoard->SetValueAsFloat(BlackBoardKey_Distance.SelectedKeyName, 0.0f);
			}
		}
	}	
}
