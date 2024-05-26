// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameCore/TowerDefenceGameState.h"
#include "../StoneDefenceMacro.h"

static FCharacterData CharacterDataNULL;

const FCharacterData& ATowerDefenceGameState::AddCharacterData(const FString &Hash, const FCharacterData& Data)
{
	return CharacterDatas.Add(Hash, Data);
}

bool ATowerDefenceGameState::RemoveCharacterData(const FString& Hash)
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

FCharacterData& ATowerDefenceGameState::GetCharacterData(const FString& Hash)
{
	if (CharacterDatas.Contains(Hash))
	{
		return CharacterDatas[Hash];
	}

	SD_print_r(Error, "The current [%s] is invalid", *Hash);
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
