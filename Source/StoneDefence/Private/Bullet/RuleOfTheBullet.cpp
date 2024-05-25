// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet/RuleOfTheBullet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Character/Core/RuleOfTheCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"

// Sets default values
ARuleOfTheBullet::ARuleOfTheBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootBullet = CreateDefaultSubobject<USceneComponent>(TEXT("BulletRootBullet"));
	BoxDamage = CreateDefaultSubobject<USphereComponent>(TEXT("BulletNoxDamage"));
	ProjectileMoement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BulletProjectileMovement"));

	RootComponent = RootBullet;
	BoxDamage->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
	ProjectileMoement->MaxSpeed = 2000.f;//最大移动速度
	ProjectileMoement->InitialSpeed = 1600.0f;//初始移动速度
	ProjectileMoement->ProjectileGravityScale = 0.0f;//重力为0，不会有掉落效果
	ProjectileMoement->UpdatedComponent = BoxDamage;
	
	BulletType = EBulletType::BULLET_DIRECT_LINE;//设置默认子弹类型
	InitialLifeSpan = 4.0f;//4S后自动销毁
}

// Called when the game starts or when spawned
void ARuleOfTheBullet::BeginPlay()
{
	Super::BeginPlay();

	switch (BulletType)
	{
	case EBulletType::BULLET_DIRECT_LINE:
		break;
	case EBulletType::BULLET_LINE:
		break;
	case EBulletType::BULLET_TRACK_LINE://跟踪子弹
		ProjectileMoement->bIsHomingProjectile = true;//开启跟踪
		ProjectileMoement->bRotationFollowsVelocity = true;//旋转跟随速度
		break;
	case EBulletType::BULLET_RANGE://范围伤害
		ProjectileMoement->StopMovementImmediately();//移动立即停止
		break;
	case EBulletType::BULLET_CHAIN://链式子弹
		ProjectileMoement->StopMovementImmediately();
		BoxDamage->SetCollisionEnabled(ECollisionEnabled::NoCollision);//设置没有任何碰撞,因为通过其他点对点的方式造成伤害，没有必要通过碰撞造成伤害
		break;
	}
	//绑定代理
	BoxDamage->OnComponentBeginOverlap.AddUniqueDynamic(this, &ARuleOfTheBullet::OnBeginOverlap);
}

// Called every frame
void ARuleOfTheBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARuleOfTheBullet::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//获取施法者
	if (ARuleOfTheCharacter* InstigatorCharacter = GetInstigator<ARuleOfTheCharacter>())
	{
		//获取碰撞者
		if (ARuleOfTheCharacter* OtherCharacter = Cast<ARuleOfTheCharacter>(Other))
		{
			//判断是不是同一个队伍
			if (InstigatorCharacter->IsTeam() != OtherCharacter->IsTeam())
			{
				//是不是活跃的
				if (OtherCharacter->IsActive())
				{
					//生成粒子特效
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DamgeParticle, SweepResult.Location);

					//造成伤害
					UGameplayStatics::ApplyDamage(OtherCharacter, 100.f, InstigatorCharacter->GetController(), InstigatorCharacter, UDamageType::StaticClass());
				}
			}
		}
	}
}

