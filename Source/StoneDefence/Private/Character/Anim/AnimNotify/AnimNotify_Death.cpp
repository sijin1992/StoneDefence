// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Anim/AnimNotify/AnimNotify_Death.h"
#include "Components/SkeletalMeshComponent.h"
//需要添加"Persona"模块
#if WITH_EDITOR
#include "Persona/Public/AnimationEditorPreviewActor.h"
#endif

UAnimNotify_Death::UAnimNotify_Death()
	:Super()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(0, 0, 0, 255);
#endif // WITH_EDITORONLY_DATA
}

void UAnimNotify_Death::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
#if WITH_EDITOR
	AAnimationEditorPreviewActor* Character = Cast<AAnimationEditorPreviewActor>(MeshComp);
	if (!Character)
	{
		if (AActor* CharacterActor = Cast<AActor>(MeshComp->GetOuter()))
		{
			CharacterActor->Destroy();
		}
	}

#else
	if (AActor* Character = Cast<AActor>(MeshComp->GetOuter()))//动画编辑器也可见，必须是AActor类型
	{
		Character->Destroy();
	}
#endif
}
