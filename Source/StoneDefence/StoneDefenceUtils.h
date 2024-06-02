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

	template<class A, class B>
	void GetAllActor(UWorld* World, TArray<B*>& Array)
	{
		for (TActorIterator<A>It(World, A::StaticClass()); It; ++It)
		{
			if (B* b = Cast<B>(*It))
			{
				Array.Add(b);
			}
		}
	}

	//获取所有的Actor
	template<class Type>
	void GetAllActor(UWorld* World, TArray<Type*>& Array)
	{
		for (TActorIterator<Type>It(World, Type::StaticClass()); It; ++It)
		{
			if (Type* A = Cast<Type>(*It))
			{
				Array.Add(A);
			}
		}
	}

	//获取所有的Actor,适合比较少的数组
	template<class Type>
	TArray<Type*> GetAllActor(UWorld* World)
	{
		TArray<Type*> Array;
		for (TActorIterator<Type>It(World, Type::StaticClass()); It; ++It)
		{
			if (Type* A = Cast<Type>(*It))
			{
				Array.Add(A);
			}
		}
		return Array;
	}
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
