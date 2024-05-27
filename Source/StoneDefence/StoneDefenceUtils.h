// Copyright Epic Games, Inc. All Rights Reserved.
//自定义工具类
#pragma once

#include "CoreMinimal.h"

class ARuleOfTheCharacter;
class IRuleCharacter;
namespace StoneDefenceUtils
{
	//找到最近的敌人
	ARuleOfTheCharacter* FindTargetRecently(const TArray<ARuleOfTheCharacter*>& InCharacters, const FVector &CompareLocation);
}

/// <summary>
/// 减伤
/// </summary>
namespace Expression
{
	float GetDamage(IRuleCharacter* Enemy, IRuleCharacter* Owner);
}
