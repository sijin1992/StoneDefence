// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Core/RuleOfTheCharacter.h"
#include "Components/SceneComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "UMG/Public/Components/WidgetComponent.h"
#include "UI/Character/UI_Health.h"
#include "../StoneDefenceUtils.h"
#include "Actor/DrawText.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/TypeData/ParticleModuleTypeDataMesh.h"
#include "Particles/ParticleEmitter.h"
#include "Particles/ParticleLODLevel.h"
#include "../StoneDefenceMacro.h"

// Sets default values
ARuleOfTheCharacter::ARuleOfTheCharacter()
	:GUID(FGuid::NewGuid()),
	bAttack(false)
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

	TraceShowCharacterInformation->OnClicked.AddDynamic(this, &ARuleOfTheCharacter::OnClicked);
	
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
	//计算伤害
	float DamageValue = Expression::GetDamage(Cast<ARuleOfTheCharacter>(DamageCauser), this);

	GetCharacterData().Health -= DamageValue;

	if (!IsActive())
	{
		GetCharacterData().Health = 0.0f;
	}

	if (DrawTextClass)
	{
		if (ADrawText* MyValueText = GetWorld()->SpawnActor<ADrawText>(DrawTextClass, GetActorLocation(),FRotator::ZeroRotator))
		{
			FString DamageText = FString::Printf(TEXT("-%0.f"), DamageValue);
			MyValueText->SetTextBlock(DamageText, FLinearColor::Red, DamageValue / GetCharacterData().MaxHealth);
		}
	}

	UpdateUI();

	return DamageValue;
}

void ARuleOfTheCharacter::OnClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	//SD_print_s("Hello");
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
		return GetGameState()->GetCharacterData(GUID);
	}
	return CharacterDataNULL;
}

UStaticMesh* ARuleOfTheCharacter::GetDollMesh(FTransform& InTransform)
{
	TArray<USceneComponent*> SceneComponents;
	RootComponent->GetChildrenComponents(true, SceneComponents);
	for (auto &TempScene : SceneComponents)
	{
		if (UStaticMeshComponent* NewMeshComponent = Cast<UStaticMeshComponent>(TempScene))
		{
			if (NewMeshComponent->GetStaticMesh())
			{
				InTransform = NewMeshComponent->GetComponentTransform();
				return NewMeshComponent->GetStaticMesh();
			}
		}
		else if(UParticleSystemComponent* NewParticleSystemComponent = Cast<UParticleSystemComponent>(TempScene))
		{
			//如果粒子特效的Template有效且发射器数量大于0
			if (NewParticleSystemComponent->Template && NewParticleSystemComponent->Template->Emitters.Num() > 0)
			{
				//遍历发射器
				for (const UParticleEmitter* TempEmitter : NewParticleSystemComponent->Template->Emitters)
				{
					if (TempEmitter->LODLevels[0]->bEnabled)//细节层次是否开启
					{
						//获取粒子特效的数据类型，看看是否可以转成模型
						if (UParticleModuleTypeDataMesh* MyParticleDataMesh = Cast<UParticleModuleTypeDataMesh>(TempEmitter->LODLevels[0]->TypeDataModule))
						{
							if (MyParticleDataMesh->Mesh)
							{
								InTransform = NewParticleSystemComponent->GetComponentTransform();
								return MyParticleDataMesh->Mesh;
							}
						}
					}
				}
			}
		}
		else if (USkeletalMeshComponent* NewSkeletalMeshComponent = Cast<USkeletalMeshComponent>(TempScene))
		{
			FString SkeletalTargetName = TEXT("CharacterMesh0");
			if (NewSkeletalMeshComponent->GetName() == SkeletalTargetName)
			{
				InTransform = NewSkeletalMeshComponent->GetComponentTransform();
				//将生成的模型单位化
				NewSkeletalMeshComponent->SetRelativeTransform(FTransform::Identity);
				NewSkeletalMeshComponent->SetWorldTransform(FTransform::Identity);
				NewSkeletalMeshComponent->SetRelativeRotation(InTransform.GetRotation());
				if (UStaticMesh* NewMesh = MeshUtils::SkeletalMeshToStaticMesh(GetWorld(), NewSkeletalMeshComponent))
				{
					return NewMesh;
				}
			}
		}
	}
	return NULL;
}
