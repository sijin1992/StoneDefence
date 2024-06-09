// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterCore/Monsters.h"
#include "Global/UI_Datas.h"
#include "GameFramework/CharacterMovementComponent.h"

void AMonsters::BeginPlay()
{
	Super::BeginPlay();
}

void AMonsters::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//GetCharacterMovement()->MaxWalkSpeed = GetCharacterData().GetWalkSpeed();
}

void AMonsters::RegisterTeam()
{
	GetCharacterData().Team = ETeam::BLUE;
}

void AMonsters::OnClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	Super::OnClicked(TouchedComponent, ButtonPressed);
	if (ClickedTargetMonster)
	{
		ClickedTargetMonster = nullptr;
	}
	else
	{
		ClickedTargetMonster = this;
	}
	
}
