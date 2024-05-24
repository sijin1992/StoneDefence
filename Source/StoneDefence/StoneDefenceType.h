// Copyright Epic Games, Inc. All Rights Reserved.
//自定义游戏类型，通过复制StoneDefence.h文件并重命名得来
#pragma once

#include "CoreMinimal.h"
#include "StoneDefenceType.generated.h"

UENUM()
//角色类型
namespace EGameCharacterType
{
	enum Type
	{
		TOWER,
		MAIN_TOWER,
		MONSTER,
		BOSS_MONSTER,
		MAX
	};
}
