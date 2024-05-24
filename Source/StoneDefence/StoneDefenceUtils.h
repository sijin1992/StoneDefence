// Copyright Epic Games, Inc. All Rights Reserved.
//自定义工具类
#pragma once

#include "CoreMinimal.h"

class ARuleOfTheCharacter;
namespace StoneDefenceUtils
{
	//找到最近的敌人
	ARuleOfTheCharacter* FindTargetRecently(const TArray<ARuleOfTheCharacter*>& InCharacters, const FVector &CompareLocation);
}
