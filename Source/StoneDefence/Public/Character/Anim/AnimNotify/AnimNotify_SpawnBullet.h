// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Bullet/RuleOfTheBullet.h"
#include "AnimNotify_SpawnBullet.generated.h"

/**
 * 
 */
UCLASS(const, hidecategories = Object, collapsecategories, Config = Game, meta = (DisplayName = "Spawn Bullet"))
class STONEDEFENCE_API UAnimNotify_SpawnBullet : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_SpawnBullet();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = (ExposeOnSpawn = true))//可在编辑器里浏览
	TSubclassOf<ARuleOfTheBullet> BulletClass;
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = (ExposeOnSpawn = true))
	FName InSocketName;
#endif

	// Begin UAnimNotify interface
	virtual FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
