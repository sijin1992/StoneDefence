// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Data/Core/CharacterData.h"
#include "TowerDefenceGameState.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API ATowerDefenceGameState : public AGameState
{
	GENERATED_BODY()

public:
	const FCharacterData& AddCharacterData(const FString &Hash, const FCharacterData &Data);

	bool RemoveCharacterData(const FString& Hash);//Hash即ID

	FCharacterData& GetCharacterData(const FString& Hash);

private:
	UPROPERTY()
	TMap<FString, FCharacterData> CharacterDatas;
};
