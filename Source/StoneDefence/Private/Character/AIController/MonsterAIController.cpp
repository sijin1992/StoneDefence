// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AIController/MonsterAIController.h"
#include "Character/CharacterCore/Towers.h"
#include "EngineUtils.h"
#include "../StoneDefenceUtils.h"

AActor* AMonsterAIController::FindTarget()
{
	TArray<ARuleOfTheCharacter*> TargetMainTowersArray;
	TArray<ARuleOfTheCharacter*> TargetTowersArray;

	for (TActorIterator<ATowers>it(GetWorld(), ATowers::StaticClass()); it; ++it)
	{
		ATowers* TheCharacter = *it;
		if (TheCharacter && TheCharacter->IsActive())
		{
			if (TheCharacter->GetType() == EGameCharacterType::Type::TOWER)
			{
				TargetTowersArray.Add(TheCharacter);
			}
			else if (TheCharacter->GetType() == EGameCharacterType::Type::MAIN_TOWER)
			{
				TargetMainTowersArray.Add(TheCharacter);
			}
		}
	}

	ATowers* MainTowers = Cast<ATowers>(StoneDefenceUtils::FindTargetRecently(TargetMainTowersArray,GetPawn()->GetActorLocation()));
	ATowers* NorTowers = Cast<ATowers>(StoneDefenceUtils::FindTargetRecently(TargetTowersArray,GetPawn()->GetActorLocation()));

	//如果主塔没死，优先返回主塔
	if (MainTowers)
	{
		return MainTowers;
	}
	return NorTowers;
}

void AMonsterAIController::AttackTarget(ARuleOfTheCharacter* AttackTarget)
{

}
