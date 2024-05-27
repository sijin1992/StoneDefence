// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Core/RuleOfTheCharacter.h"
#include "Components/SceneComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "UMG/Public/Components/WidgetComponent.h"
#include "UI/Character/UI_Health.h"
#include "../StoneDefenceUtils.h"

// Sets default values
ARuleOfTheCharacter::ARuleOfTheCharacter()
	:bAttack(false)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HomingPoint = CreateDefaultSubobject<USceneComponent>(TEXT("HomingPoint"));
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	OpenFirePoint = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPoint"));
	TraceShowCharacterInformation = CreateDefaultSubobject<UBoxComponent>(TEXT("TraceBox"));

	HomingPoint->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Widget->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	OpenFirePoint->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	TraceShowCharacterInformation->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//设置碰撞通道的预设Scanning
	TraceShowCharacterInformation->SetCollisionProfileName("Scanning");
	//设置碰撞盒大小
	TraceShowCharacterInformation->SetBoxExtent(FVector(38, 38, 100));
}

// Called when the game starts or when spawned
void ARuleOfTheCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//生成角色需要生成一个默认的控制
	if (!GetController())
	{
		SpawnDefaultController();
	}

	UpdateUI();
}

// Called every frame
void ARuleOfTheCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARuleOfTheCharacter::UpdateUI()
{
	if (Widget)
	{
		if (UUI_Health* HealthUI = Cast<UUI_Health>(Widget->GetUserWidgetObject()))
		{
			HealthUI->SetTitle(GetCharacterData().Name.ToString());
			HealthUI->SetHealth(GetHealth() / GetMaxHealth());
		}
	}
}

EGameCharacterType::Type ARuleOfTheCharacter::GetType()
{
	return EGameCharacterType::Type::MAX;
}

float ARuleOfTheCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	float DamageValue = Expression::GetDamage(Cast<ARuleOfTheCharacter>(DamageCauser), this);

	GetCharacterData().Health -= DamageValue;

	if (!IsActive())
	{
		GetCharacterData().Health = 0.0f;
	}

	UpdateUI();

	return DamageValue;
}

bool ARuleOfTheCharacter::IsDeath()
{
	return GetHealth() <= 0.0f;
}

float ARuleOfTheCharacter::GetHealth()
{
	return GetCharacterData().Health;
}

float ARuleOfTheCharacter::GetMaxHealth()
{
	return GetCharacterData().MaxHealth;
}

bool ARuleOfTheCharacter::IsTeam()
{
	return false;
}

FCharacterData& ARuleOfTheCharacter::GetCharacterData()
{
	if (GetGameState())
	{
		return GetGameState()->GetCharacterData(GetUniqueID());
	}
	return CharacterDataNULL;
}
