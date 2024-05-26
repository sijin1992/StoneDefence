// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Data/Core/CharacterData.h"
#include "TowerDefenceGameState.generated.h"

FCharacterData CharacterDataNULL;
class ARuleOfTheCharacter;
/**
 * 
 */
UCLASS()
class STONEDEFENCE_API ATowerDefenceGameState : public AGameState
{
	GENERATED_BODY()

public:
	ATowerDefenceGameState();

	//生成角色
	ARuleOfTheCharacter* SpawnCharacter(const FVector& Location, const FRotator& Rotator);

	template<class T>
	T *SpawnCharacter(const FVector& Location, const FRotator& Rotator)
	{
		return Cast<T>(SpawnCharacter(Location, Rotator));
	}
	//增
	const FCharacterData& AddCharacterData(const FGuid &Hash, const FCharacterData &Data);
	//删
	bool RemoveCharacterData(const FGuid &Hash);//Hash即ID
	//改
	FCharacterData& GetCharacterData(const FGuid &Hash);

private:
	UPROPERTY()
	TMap<FGuid, FCharacterData> CharacterDatas;
};
