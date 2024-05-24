// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AIController/MonsterAIController.h"
#include "Character/CharacterCore/Towers.h"
#include "EngineUtils.h"

AActor* AMonsterAIController::FindTarget()
{
	//获取最近塔的算法
	//[&]:GetPawn方法是MonsterController里的，所以要在[]加入&
	//->ATowers：Lambda表达式中带有返回值类型
	auto GetRecentlyTowers = [&](const TArray<ATowers*> &MyTowers) -> ATowers*
		{
			if (MyTowers.Num())
			{
				float TowerTargetDistance = 99999999.0f;
				int32 TowersIndex = INDEX_NONE;//-1
				FVector MyLocation = GetPawn()->GetActorLocation();
				for (int32 i = 0; i < MyTowers.Num(); i++)
				{
					if (ATowers* TowerCharacter = MyTowers[i])
					{
						FVector TowerLocation = TowerCharacter->GetActorLocation();
						FVector TmpVector = TowerLocation - MyLocation;
						float TowerAndMonsterDistance = TmpVector.Size();

						if (TowerAndMonsterDistance < TowerTargetDistance)
						{
							TowersIndex = i;
							TowerTargetDistance = TowerAndMonsterDistance;
						}
					}
				}

				if (TowersIndex != INDEX_NONE)
				{
					return MyTowers[TowersIndex];
				}
			}

			return NULL;
		};

	TArray<ATowers*> TargetMainTowersArray;
	TArray<ATowers*> TargetTowersArray;

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

	ATowers* MainTowers = GetRecentlyTowers(TargetMainTowersArray);
	ATowers* NorTowers = GetRecentlyTowers(TargetTowersArray);

	//如果主塔没死，优先返回主塔
	if (MainTowers)
	{
		return MainTowers;
	}
	return NorTowers;
}
