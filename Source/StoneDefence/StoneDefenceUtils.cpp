#include "StoneDefenceUtils.h"
#include "Public/Character/Core/RuleOfTheCharacter.h"
#include "Interface/Character/RuleCharacter.h"
//关闭优化optimize
#if PLATFORM_WINDOWS
#pragma optimize("",off)
#endif

ARuleOfTheCharacter* StoneDefenceUtils::FindTargetRecently(const TArray<ARuleOfTheCharacter*> &InCharacters, const FVector& CompareLocation)
{
	if (InCharacters.Num())
	{
		float TargetDistance = 99999999.0f;
		int32 TargetIndex = INDEX_NONE;//-1
		for (int32 i = 0; i < InCharacters.Num(); i++)
		{
			if (ARuleOfTheCharacter* TargetCharacter = InCharacters[i])
			{
				FVector TargetLocation = TargetCharacter->GetActorLocation();
				FVector TmpVector = TargetLocation - CompareLocation;
				float TargetAndSelfDistance = TmpVector.Size();

				if (TargetAndSelfDistance < TargetDistance && TargetCharacter->IsActive())
				{
					TargetIndex = i;
					TargetDistance = TargetAndSelfDistance;
				}
			}
		}

		if (TargetIndex != INDEX_NONE)
		{
			return InCharacters[TargetIndex];
		}
	}

	return NULL;
}

float Expression::GetDamage(IRuleCharacter* Enemy, IRuleCharacter* Owner)
{
	if (Enemy && Owner)
	{
		return Enemy->GetCharacterData().PhysicalAttack / ((Owner->GetCharacterData().Armor / 100.0f) + 1);
	}
	return 0.0f;
}

//打开优化
#if PLATFORM_WINDOWS
#pragma optimize("",on)
#endif
