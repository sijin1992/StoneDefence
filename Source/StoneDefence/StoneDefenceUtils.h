// Copyright Epic Games, Inc. All Rights Reserved.
//自定义工具类
#pragma once

#include "CoreMinimal.h"

class ARuleOfTheCharacter;
class IRuleCharacter;
class UStaticMesh;
class USkeletalMeshComponent;
class UWorld;
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
/// <summary>
/// 对模型进行操作
/// </summary>
namespace MeshUtils
{
	/// <summary>
	/// 将SkeletalMesh转换成StaticMesh
	/// </summary>
	/// <param name="SkeletalMeshComponent"></param>
	/// <returns></returns>
	UStaticMesh* SkeletalMeshToStaticMesh(UWorld* InWorld, USkeletalMeshComponent* SkeletalMeshComponent);
}
