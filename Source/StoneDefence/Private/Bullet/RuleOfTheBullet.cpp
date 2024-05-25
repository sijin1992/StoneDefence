// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet/RuleOfTheBullet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Character/Core/RuleOfTheCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "Character/Core/RuleOfTheAIController.h"
#include "EngineUtils.h"

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

	//处理不同子弹类型初始化
	switch (BulletType)
	{
	case EBulletType::BULLET_DIRECT_LINE:
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OpenFireParticle, GetActorLocation());//播放开火特效
		break;
	case EBulletType::BULLET_LINE:
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OpenFireParticle, GetActorLocation());
		break;
	case EBulletType::BULLET_TRACK_LINE://跟踪子弹
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OpenFireParticle, GetActorLocation());
		ProjectileMoement->bIsHomingProjectile = true;//开启跟踪
		ProjectileMoement->bRotationFollowsVelocity = true;//旋转跟随速度
		//拿到施法者
		if (ARuleOfTheCharacter *InstigatorCharacter = (GetInstigator<ARuleOfTheCharacter>()))
		{
			//拿到Controller
			if (ARuleOfTheAIController *InstigatorController = Cast<ARuleOfTheAIController>(InstigatorCharacter->GetController()))
			{
				//拿到Target
				if (ARuleOfTheCharacter* TargetCharacter = InstigatorController->Target.Get())
				{
					//对当前目标加速度大小设置
					ProjectileMoement->HomingAccelerationMagnitude = 4000.0f;
					//跟踪组件设为目标的跟踪点
					ProjectileMoement->HomingTargetComponent = TargetCharacter->GetHomingPoint();
				}
			}
		}

		break;
	}
	case EBulletType::BULLET_RANGE_LINE://范围伤害,丢手雷
		break;
	case EBulletType::BULLET_RANGE://范围伤害
		//获取施法者
		if (ARuleOfTheCharacter* InstigatorCharacter = GetInstigator<ARuleOfTheCharacter>())
		{
			ProjectileMoement->StopMovementImmediately();//移动立即停止
			
			TArray<AActor*> IgnoreActors;//忽略/友军列表
			//TArray<AActor*> TargetActors;//敌人列表
			//迭代器遍历忽略的Actor和目标Actor
			for (TActorIterator<ARuleOfTheCharacter>it(GetWorld(), ARuleOfTheCharacter::StaticClass()); it; ++it)
			{
				if (ARuleOfTheCharacter* TheCharacter = *it)
				{
					FVector VDistance = TheCharacter->GetActorLocation() - InstigatorCharacter->GetActorLocation();
					if (VDistance.Size() <= 1400.0f)
					{
						if (TheCharacter->IsTeam() == InstigatorCharacter->IsTeam())
						{
							IgnoreActors.Add(TheCharacter);
						}
						else
						{
							UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DamgeParticle, TheCharacter->GetActorLocation());
							//TargetActors.Add(TheCharacter);
						}
					}
				}
			}
			//造成范围伤害,向四周衰减
			UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), 100.0f, 10.0f, GetActorLocation(), 400.0f, 1000.0f, 1.0f, UDamageType::StaticClass(), IgnoreActors, GetInstigator());
		}
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

				//处理不同子弹类型销毁
				switch (BulletType)
				{
				case EBulletType::BULLET_LINE:
				case EBulletType::BULLET_TRACK_LINE:
					Destroy();
					break;
				}
			}
		}
	}
}

