#include "StoneDefenceUtils.h"
#include "Public/Character/Core/RuleOfTheCharacter.h"

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

