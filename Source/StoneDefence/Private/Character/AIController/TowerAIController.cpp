// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AIController/TowerAIController.h"
#include "EngineUtils.h"
#include "Character/CharacterCore/Monsters.h"
#include "Character/CharacterCore/Towers.h"
#include "../StoneDefenceUtils.h"
#include "UObject/WeakObjectPtrTemplates.h"

ATowerAIController::ATowerAIController()
	:HeartbeatDiagnosis(0.0f)
{
}

void ATowerAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//心跳诊断
	HeartbeatDiagnosis += DeltaTime;
	if (HeartbeatDiagnosis >= 0.5f)
	{
		BTService_FindTarget();
		HeartbeatDiagnosis = 0;
	}
	//遍历怪物列表
	if (TArrayMonsters.Num())
	{
		if (ATowers *Towers = GetPawn<ATowers>())
		{
			if (!TargetWeakPtr.IsValid())
			{
				TargetWeakPtr = Cast<ARuleOfTheCharacter>(FindTarget());
			}

			if (TargetWeakPtr.IsValid())
			{
				Towers->TowersRotator = FRotationMatrix::MakeFromX(TargetWeakPtr->GetActorLocation() - GetPawn()->GetActorLocation()).Rotator();
				if (GetPawn()->GetActorRotation() != FRotator::ZeroRotator)
				{
					Towers->TowersRotator -= GetPawn()->GetActorRotation();
				}
			}
		}
	}
}

AActor* ATowerAIController::FindTarget()
{
	if (TArrayMonsters.Num())
	{
		return StoneDefenceUtils::FindTargetRecently(TArrayMonsters, GetPawn()->GetActorLocation());
	}
	return nullptr;
}

void ATowerAIController::AttackTarget(ARuleOfTheCharacter* AttackTarget)
{
	if (ATowers *Towers = GetPawn<ATowers>())
	{
		if (TArrayMonsters.Num() > 0)
		{
			Towers->bAttack = true;
		}
		else
		{
			Towers->bAttack = false;
		}
	}
}

void ATowerAIController::BTService_FindTarget()
{
	//清空目标怪物列表
	TArrayMonsters.Empty();
	if (ATowers* Towers = GetPawn<ATowers>())
	{
		//判断塔自身是否还活着
		if (Towers->IsActive())
		{
			for (TActorIterator<AMonsters>it(GetWorld(), AMonsters::StaticClass()); it; ++it)
			{
				if (AMonsters* TheCharacter = *it)
				{
					if (TheCharacter->IsActive())
					{
						FVector TDistance = TheCharacter->GetActorLocation() - GetPawn()->GetActorLocation();
						if (TDistance.Size() <= 1600)
						{
							TArrayMonsters.Add(TheCharacter);
						}
					}
				}
			}

			AttackTarget(TargetWeakPtr.Get());//弱指针Target.Get：将弱指针转换成裸指针
		}
	}
}
