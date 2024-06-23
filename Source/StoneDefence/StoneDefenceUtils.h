// Copyright Epic Games, Inc. All Rights Reserved.
//自定义工具类
#pragma once

#include "CoreMinimal.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "UMG/Public/Components/SizeBox.h"

class ARuleOfTheCharacter;
class IRuleCharacter;
class UStaticMesh;
class USkeletalMeshComponent;
class UWorld;
class AStaticMeshActor;
class ATowerDefencePlayerController;
class ARuleOfTheBullet;
class APlayerSkillSlotActor;

namespace StoneDefenceUtils
{
	//获取存档
	template<class T>
	T* GetSave(UWorld* InWorld, const TCHAR* SaveName, int32 SaveIndex = INDEX_NONE, EGameSaveType InFlag = EGameSaveType::NONE)
	{
		T* InSlot = nullptr;

		auto InitSlot = [&]()
			{
				InSlot = Cast<T>(UGameplayStatics::CreateSaveGameObject(T::StaticClass()));
				if (InSlot)
				{
					InSlot->InitSaveGame(InWorld);
				}
			};

		if (InFlag && EGameSaveType::ARCHIVES)
		{
			FString SlotString = SaveName;
			if (SaveIndex != INDEX_NONE)
			{
				//FText MyText = FText::Format(FText::FromString(SaveName), SaveIndex);
				//SlotString = MyText.ToString();
				if (SlotString.Contains("%i"))
				{
					SlotString.RemoveFromEnd("%i");
					SlotString += FString::FromInt(SaveIndex);
				}
			}
			else
			{
				if (SlotString.Contains("%i"))
				{
					SlotString.RemoveFromEnd("%i");
					SlotString += TEXT("0");
				}
			}

			InSlot = Cast<T>(UGameplayStatics::LoadGameFromSlot(SlotString, 0));
			if (!InSlot)
			{
				InitSlot();
			}
		}
		else
		{
			InitSlot();
		}

		return InSlot;
	}

	//切换界面的模板
	template<class T, class UserObject>
	UserObject* CreateAssistWidget(T* ThisClass, UClass* AssistClass, USizeBox* WidgetArray)
	{
		UserObject* UserObjectElement = nullptr;

		//播放动画的判断
		if (0)
		{
			//播放淡入
		}

		if (WidgetArray->GetChildAt(0))
		{
			if (WidgetArray->GetChildAt(0)->IsA(AssistClass))
			{
				//关闭Border淡出

				return UserObjectElement;
			}
			else
			{
				WidgetArray->ClearChildren();
			}
		}

		UserObjectElement = CreateWidget<UserObject>(ThisClass->GetWorld(), AssistClass);
		if (UserObjectElement)
		{
			if (WidgetArray->AddChild(UserObjectElement))
			{
			}
			else
			{
				UserObjectElement->RemoveFromParent();
			}
		}

		return UserObjectElement;
	}

	//找到相应角色触发代理函数
	void FindCharacterToExecution(UWorld* InWorld, const FGuid CharacterFGuid, TFunction<void(ARuleOfTheCharacter* InCharacter)> Code);

	//通知客户端调用代理函数,多播委托
	void CallUpdateAllClient(UWorld* InWorld, TFunction<void(ATowerDefencePlayerController* MyPlayerController)> InImplement);

	//生成玩家技能
	APlayerSkillSlotActor* SpawnPlayerSkill(UWorld* InWorld, int32 SkillID);

	//服务端通知客户端生成子弹
	ARuleOfTheBullet* SpawnBullet(UWorld* InWorld, FGuid CharacterFGuid, UClass* InClass);
	//客户端生成子弹
	ARuleOfTheBullet* SpawnBullet(UWorld* InWorld, APawn* NewPawn, UClass* InClass, const FVector& InLocation, const FRotator& InRotation);

	ARuleOfTheBullet* SpawnBullet(UWorld* InWorld, ARuleOfTheCharacter* Instigator, const int32 SkillID, const FVector& InLocation, const FRotator& InRotation);

	//生成炮塔的绿色模型
	AStaticMeshActor* SpawnTowersDoll(UWorld* InWorld, int32 ID);

	//寻找一定范围内最近的人
	void FindRangeTargetRecently(ARuleOfTheCharacter* InOwner, float Range, TArray<ARuleOfTheCharacter*> &Targets);

	//找到最近的敌人
	ARuleOfTheCharacter* FindTargetRecently(const TArray<ARuleOfTheCharacter*>& InCharacters, const FVector &CompareLocation);

	template<class A, class B>
	void GetAllActor(UWorld* World, TArray<B*>& Array)
	{
		for (TActorIterator<A>It(World, A::StaticClass()); It; ++It)
		{
			if (B* b = Cast<A>(*It))
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
			Array.Add(*It);
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
	UStaticMesh* ParticleSystemToStaticMesh(UParticleSystemComponent* NewParticleSystemComponent);
	/// <summary>
	/// 将SkeletalMesh转换成StaticMesh
	/// </summary>
	/// <param name="SkeletalMeshComponent"></param>
	/// <returns></returns>
	UStaticMesh* SkeletalMeshToStaticMesh(USkeletalMeshComponent* SkeletalMeshComponent);
}
