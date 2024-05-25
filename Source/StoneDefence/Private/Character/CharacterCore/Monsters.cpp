// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterCore/Monsters.h"

void AMonsters::BeginPlay()
{
	Super::BeginPlay();
}

void AMonsters::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AMonsters::IsTeam()
{
	return Super::IsTeam();
}

EGameCharacterType::Type AMonsters::GetType()
{
	return EGameCharacterType::Type::MONSTER;
}
