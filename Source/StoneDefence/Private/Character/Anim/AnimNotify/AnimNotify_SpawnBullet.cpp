// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Anim/AnimNotify/AnimNotify_SpawnBullet.h"
#include "Character/Core/RuleOfTheCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "../StoneDefenceUtils.h"

UAnimNotify_SpawnBullet::UAnimNotify_SpawnBullet()
	:Super()
{
#if WITH_EDITORONLY_DATA
	InSocketName = TEXT("OpenFire");
	NotifyColor = FColor(196, 142, 255, 255);
#endif // WITH_EDITORONLY_DATA
}

FString UAnimNotify_SpawnBullet::GetNotifyName_Implementation() const
{
	if (BulletClass)
	{
		return BulletClass->GetName();
	}
	else
	{
		return Super::GetNotifyName_Implementation();
	}
}

void UAnimNotify_SpawnBullet::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
#if WITH_EDITOR
	FVector ComponentLocation = MeshComp->GetSocketLocation(InSocketName);
	FRotator ComponentRotation = MeshComp->GetSocketRotation(InSocketName);
	if (AActor* Character = Cast<AActor>(MeshComp->GetOuter()))//动画编辑器也可见，必须是AActor类型
#else
	FVector ComponentLocation = Character->GetOpenFirePoint()->GetComponentLocation();
	FRotator ComponentRotation = Character->GetOpenFirePoint()->GetComponentRotation();
	if (ARuleOfTheCharacter* Character = Cast<ARuleOfTheCharacter>(MeshComp->GetOuter()))//动画编辑器看不到，必须Runtime状态才能看见生成的Actor
#endif
	{
		StoneDefenceUtils::SpawnBullet(Character->GetWorld(), Cast<APawn>(Character), BulletClass, ComponentLocation, ComponentRotation);
	}
}
