// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Anim/AnimNotify/AnimNotify_Tag.h"
#include "Character/Core/RuleOfTheCharacter.h"

UAnimNotify_Tag::UAnimNotify_Tag()
	:Super()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(255, 0, 0, 255);
#endif // WITH_EDITORONLY_DATA
}

void UAnimNotify_Tag::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (ARuleOfTheCharacter* CharacterActor = Cast<ARuleOfTheCharacter>(MeshComp->GetOuter()))
	{
		//可用于播放动画时切换材质
		CharacterActor->AnimTag();
	}

}
