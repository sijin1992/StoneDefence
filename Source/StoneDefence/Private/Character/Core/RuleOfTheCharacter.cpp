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
#include "../StoneDefenceMacro.h"

// Sets default values
ARuleOfTheCharacter::ARuleOfTheCharacter()
	:GUID(FGuid::NewGuid()),
	CharacterType(EGameCharacterType::Type::MINI),
	bAttack(false),
	DelayDeath(10.0f)
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
}

// Called every frame
void ARuleOfTheCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateUI();
}

void ARuleOfTheCharacter::UpdateUI()
{
	if (Widget)
	{
		if (GetCharacterData().IsValid())
		{
			if (UUI_Health* HealthUI = Cast<UUI_Health>(Widget->GetUserWidgetObject()))
			{
				HealthUI->SetTitle(GetCharacterData().Name.ToString());
				HealthUI->SetHealth(GetHealth() / GetMaxHealth());
			}
		}
	}
}

void ARuleOfTheCharacter::ResetGUID()
{
	GUID = FGuid::NewGuid();
}

EGameCharacterType::Type ARuleOfTheCharacter::GetCharacterType()
{
	return CharacterType;
}

/// <summary>
/// 造成伤害
/// </summary>
/// <param name="Damage">伤害</param>
/// <param name="DamageEvent"></param>
/// <param name="EventInstigator"></param>
/// <param name="DamageCauser">伤害造成者</param>
/// <returns></returns>
float ARuleOfTheCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	//绘制飘字
	auto DrawGameText = [&](ARuleOfTheCharacter* InOwer, const TCHAR* InText, float InDamageValue, FLinearColor InColor)
		{
			if (DrawTextClass)
			{
				if (ADrawText* MyValueText = GetWorld()->SpawnActor<ADrawText>(DrawTextClass, InOwer->GetActorLocation(), FRotator::ZeroRotator))
				{
					TCHAR InTextArray[256];
					FCString::Strcpy(InTextArray, InText);
					FString DamageText = FString::Printf(InTextArray, InDamageValue);
					MyValueText->SetTextBlock(DamageText, InColor, InDamageValue / InOwer->GetCharacterData().GetMaxHealth());
				}
			}
		};

	//计算伤害
	float DamageValue = Expression::GetDamage(Cast<ARuleOfTheCharacter>(DamageCauser), this);

	GetCharacterData().Health -= DamageValue;
	//死亡判定
	if (!IsActive())
	{
		//通知蓝图角色死亡
		CharacterDeath();
		//杀掉怪物获取金币
		if (GetPlayerState()->GetPlayerData().Team != GetTeamType())
		{
			GetPlayerState()->GetPlayerData().GameGold += GetCharacterData().Gold;
		}

		GetCharacterData().Health = 0.0f;
		SetLifeSpan(DelayDeath);//防止死亡动画通知没有起作用，给一个安全时间销毁

		//把UI隐藏
		Widget->SetVisibility(false);
		//谁把我杀死，我就给谁全部的经验
		if (ARuleOfTheCharacter* CauserCharacter = Cast<ARuleOfTheCharacter>(DamageCauser))
		{
			//升极
			if (CauserCharacter->IsActive())
			{
				if (CauserCharacter->GetCharacterData().UpdateEXP(GetCharacterData().AddEmpiricalValue))
				{
					//播放升极特效
				}
				//绘制经验值飘字
				DrawGameText(CauserCharacter, TEXT("+EXP %0.0f"), GetCharacterData().AddEmpiricalValue, FLinearColor::Yellow);
			}

			//寻找范围内最近的敌人
			TArray<ARuleOfTheCharacter*> EnemyCharacters;
			StoneDefenceUtils::FindRangeTargetRecently(this, 1000.0f, EnemyCharacters);
			for (ARuleOfTheCharacter* InEnemy : EnemyCharacters)
			{
				if (InEnemy != CauserCharacter)
				{
					if (InEnemy->IsActive())
					{
						if (InEnemy->GetCharacterData().UpdateEXP(GetCharacterData().AddEmpiricalValue * 0.3f))
						{
						}
						//绘制经验值飘字
						DrawGameText(InEnemy, TEXT("+EXP %0.0f"), GetCharacterData().AddEmpiricalValue, FLinearColor::Yellow);
					}
				}
			}
		}

		//把当前的角色移除
		GetGameState()->RemoveCharacterData(GUID);
	}
	//绘制受到伤害飘字
	DrawGameText(this, TEXT("-%0.0f"), DamageValue, FLinearColor::Red);

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
	return GetCharacterData().GetMaxHealth();
}

ETeam ARuleOfTheCharacter::GetTeamType()
{
	return GetCharacterData().Team;
}

FCharacterData& ARuleOfTheCharacter::GetCharacterData()
{
#if WITH_EDITOR
	if (GetGameState())
	{
		return GetGameState()->GetCharacterData(GUID);
	}
	return NULLCharacterData;
#else
	return GetGameState()->GetCharacterData(GUID);
#endif
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
			if (UStaticMesh* NewMesh = MeshUtils::ParticleSystemToStaticMesh(NewParticleSystemComponent))
			{
				InTransform = NewParticleSystemComponent->GetComponentTransform();
				return NewMesh;
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
				if (UStaticMesh* NewMesh = MeshUtils::SkeletalMeshToStaticMesh(NewSkeletalMeshComponent))
				{
					return NewMesh;
				}
			}
		}
	}
	return NULL;
}

void ARuleOfTheCharacter::AddSkillSlot_Client(const FGuid& SlotID)
{
	if (UUI_Health* HealthUI = Cast<UUI_Health>(Widget->GetUserWidgetObject()))
	{
		HealthUI->AddSkillSlot(SlotID);
	}
}

void ARuleOfTheCharacter::RemoveSkillSlot_Client(const FGuid& SlotID)
{
	if (UUI_Health* HealthUI = Cast<UUI_Health>(Widget->GetUserWidgetObject()))
	{
		HealthUI->RemoveSkillSlot(SlotID);
	}
}
