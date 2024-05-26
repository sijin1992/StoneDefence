// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameCore/TowerDefenceGameState.h"
#include "../StoneDefenceMacro.h"
#include "Character/Core/RuleOfTheCharacter.h"

ATowerDefenceGameState::ATowerDefenceGameState()
{
	
}

ARuleOfTheCharacter* ATowerDefenceGameState::SpawnCharacter(const FVector& Location, const FRotator& Rotator)
{
	if (GetWorld())
	{
		if (ARuleOfTheCharacter* RuleOfTheCharacter = GetWorld()->SpawnActor<ARuleOfTheCharacter>(ARuleOfTheCharacter::StaticClass(), Location, Rotator))
		{
			RuleOfTheCharacter->GUID = FGuid::NewGuid();
			FCharacterData CharacterData;
			AddCharacterData(RuleOfTheCharacter->GUID, CharacterData);
		}
	}

	return nullptr;
}

const FCharacterData& ATowerDefenceGameState::AddCharacterData(const FGuid &Hash, const FCharacterData &Data)
{
	return CharacterDatas.Add(Hash, Data);
}

bool ATowerDefenceGameState::RemoveCharacterData(const FGuid& Hash)
{
	if (CharacterDatas.Remove(Hash))
	{
		return true;
	}

	return false;
	/*
	FCharacterData RemoveData;
	for (auto& Tmp : CharacterDatas)
	{
		if (Tmp.GUID == Hash)
		{
			RemoveData = Tmp;
			break;
		}
	}
	for (auto&)
	{
	}
	*/
}

FCharacterData& ATowerDefenceGameState::GetCharacterData(const FGuid& Hash)
{
	if (CharacterDatas.Contains(Hash))
	{
		return CharacterDatas[Hash];
	}

	SD_print_r(Error, "The current [%s] is invalid", *Hash.ToString());
	return CharacterDataNULL;
	/*
	for (auto &Tmp : CharacterDatas)
	{
		if (Tmp.Key == Hash)
		{
			return Tmp.Value;
		}
	}
	*/
}
