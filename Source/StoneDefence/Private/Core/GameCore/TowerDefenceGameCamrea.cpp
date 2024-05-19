// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameCore/TowerDefenceGameCamrea.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ATowerDefenceGameCamrea::ATowerDefenceGameCamrea()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Boom"));
	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Main_Camera"));
	MarkBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Sign"));

	CameraBoom->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	MainCamera->AttachToComponent(CameraBoom, FAttachmentTransformRules::KeepRelativeTransform);
	MarkBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//控制我们摄像机的角度和距离地面的高度
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
}

// Called when the game starts or when spawned
void ATowerDefenceGameCamrea::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called to bind functionality to input
void ATowerDefenceGameCamrea::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATowerDefenceGameCamrea::Zoom(bool bDirection, const float& ZoomSpeed)
{
	if (bDirection)
	{
		if (CameraBoom->TargetArmLength > 400)
		{
			CameraBoom->TargetArmLength -= ZoomSpeed * 2;
		}
	}
	else
	{
		if (CameraBoom->TargetArmLength < 800)
		{
			CameraBoom->TargetArmLength += ZoomSpeed * 2;
		}
	}
}

