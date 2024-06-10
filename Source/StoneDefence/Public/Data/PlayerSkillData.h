#pragma once

#include "CoreMinimal.h"
#include "SkillData.h"
#include "PlayerSkillData.generated.h"

USTRUCT(BlueprintType)
struct FPlayerSkillData : public FSkillData
{
	GENERATED_BODY()

	FPlayerSkillData();

	virtual void Init();
	//技能数量
	UPROPERTY(EditDefaultsOnly, Category = "Skill Profit")
	int32 SkillNumber;
};