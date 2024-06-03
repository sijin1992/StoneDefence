// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_Death.generated.h"

/**
 * 
 */
UCLASS(const, hidecategories = Object, collapsecategories, Config = Game, meta = (DisplayName = "Death"))
class STONEDEFENCE_API UAnimNotify_Death : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UAnimNotify_Death();

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
