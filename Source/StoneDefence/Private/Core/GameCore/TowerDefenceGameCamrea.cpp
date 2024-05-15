// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameCore/TowerDefenceGameCamrea.h"

// Sets default values
ATowerDefenceGameCamrea::ATowerDefenceGameCamrea()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATowerDefenceGameCamrea::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATowerDefenceGameCamrea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATowerDefenceGameCamrea::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

