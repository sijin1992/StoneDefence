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
/*
USTRUCT()
struct FPlayerSkillData
{
	GENERATED_USTRUCT_BODY()

	FPlayerSkillData();

	void Init();

	bool IsValid();

	//技能ID
	UPROPERTY(SaveGame)
	int32 SkillID;
	//技能CD
	UPROPERTY(SaveGame)
	float SkillCD;
	//最大技能CD
	UPROPERTY(SaveGame)
	float MaxSkillID;
	//技能数
	UPROPERTY(SaveGame)
	int32 SkillNumber;

};



FPlayerSkillData::FPlayerSkillData()
{
	Init();
}

void FPlayerSkillData::Init()
{
	SkillID = INDEX_NONE;
	SkillCD = 0.0f;
	MaxSkillID = 0.0f;
	SkillNumber = INDEX_NONE;
}

bool FPlayerSkillData::IsValid()
{
	return SkillID != INDEX_NONE;
}
*/