// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Core/RuleOfTheAIController.h"
#include "Data/CharacterData.h"
#include "Character/Core/RuleOfTheCharacter.h"

void ARuleOfTheAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//服务器上的逻辑
	if (GetLocalRole() == ROLE_Authority)
	{
		if (ARuleOfTheCharacter* InCharacter = Cast<ARuleOfTheCharacter>(GetPawn()))
		{
			FCharacterData& Data = InCharacter->GetCharacterData();
			if (Data.IsValid())
			{
				Data.Location = InCharacter->GetActorLocation();
				Data.Rotator = InCharacter->GetActorRotation();
			}
		}
	}
}

ARuleOfTheCharacter* ARuleOfTheAIController::GetTarget()
{
	return Target.Get();
}
