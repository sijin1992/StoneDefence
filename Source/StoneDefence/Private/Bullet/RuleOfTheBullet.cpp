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
#include "Components/ArrowComponent.h"
#include "Components/SplineComponent.h"

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

	SplineOffset = 0.0f;
	Spline = nullptr;
	CurrentSplineTime = 0.0f;
}

// Called when the game starts or when spawned
void ARuleOfTheBullet::BeginPlay()
{
	Super::BeginPlay();

	//拿到施法者
	if (ARuleOfTheCharacter* InstigatorCharacter = GetInstigator<ARuleOfTheCharacter>())
	{
		//拿到Controller
		if (ARuleOfTheAIController* InstigatorController = Cast<ARuleOfTheAIController>(InstigatorCharacter->GetController()))
		{
			//拿到Target
			if (ARuleOfTheCharacter* TargetCharacter = InstigatorController->Target.Get())
			{
				//处理不同子弹类型初始化的逻辑
				switch (BulletType)
				{
					case EBulletType::BULLET_DIRECT_LINE:
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OpenFireParticle, GetActorLocation());//播放开火特效
						break;
					case EBulletType::BULLET_LINE:
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OpenFireParticle, GetActorLocation());
						break;
					case EBulletType::BULLET_TRACK_BEZIER://使用Spline组件实现贝赛尔曲线轨迹的跟踪子弹，商业项目常用
					{
						ProjectileMoement->StopMovementImmediately();//关闭动态模拟
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OpenFireParticle, GetActorLocation());//播放开火特效

						Spline = NewObject<USplineComponent>(this, TEXT("SplineInstance"));//动态创建,this参数就是把组件变为内部Ou
						Spline->RegisterComponent();//注册组件
	
						Spline->SetLocationAtSplinePoint(0, GetActorLocation(), ESplineCoordinateSpace::Local);	//设置Spline贝塞尔曲线下标为0的点

						FVector DistanceVector = InstigatorCharacter->GetActorLocation() - TargetCharacter->GetActorLocation();//距离
						FVector MidPosition = (DistanceVector / 2) + TargetCharacter->GetActorLocation();//中点距离
						MidPosition.Y += SplineOffset;
						MidPosition.Z = (DistanceVector.Size() / 2.0f) * 0.5f;//曲线高度

						Spline->SetLocationAtSplinePoint(1, MidPosition, ESplineCoordinateSpace::Local);//设置Spline下标为1的点
						Spline->AddSplinePoint(TargetCharacter->GetActorLocation(), ESplineCoordinateSpace::Local);//增加终点坐标

						break;
					}
					case EBulletType::BULLET_TRACK_LINE://使用UE的物理模拟实现跟踪子弹,不太真实
					{
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OpenFireParticle, GetActorLocation());
						ProjectileMoement->bIsHomingProjectile = true;//开启跟踪
						ProjectileMoement->bRotationFollowsVelocity = true;//旋转跟随速度
						ProjectileMoement->HomingAccelerationMagnitude = 4000.0f;//对当前目标加速度大小设置
						ProjectileMoement->HomingTargetComponent = TargetCharacter->GetHomingPoint();//跟踪组件设为目标的跟踪点
						break;
					}
					case EBulletType::BULLET_RANGE_LINE://范围伤害,丢手雷,使用UE的物理模拟实现抛物线子弹
					{
						ProjectileMoement->StopMovementImmediately();//关闭动态模拟
						ProjectileMoement->ProjectileGravityScale = 1.0f;//打开重力
						FVector TargetFromOwnerVector = TargetCharacter->GetActorLocation() - GetActorLocation();//距离d,与水平面可能存在误差

						float InTime = (TargetFromOwnerVector.Size() / ProjectileMoement->InitialSpeed);//时间t
						float Y = ProjectileMoement->GetGravityZ() * InTime;//Vy,GetGravityZ,UE中的重力加速度是每秒980像素
						float X = ProjectileMoement->InitialSpeed * InTime;//Vx
						float V = FMath::Sqrt(X * X + Y * Y);//V
						float CosRadian = FMath::Acos(TargetFromOwnerVector.Size() / V * (InTime * (PI * 0.1f)));//反余弦求出抛物线弧度,PI * 0.1f用于修复上面的误差
						FRotator Rot = GetActorRotation();
						Rot.Pitch = CosRadian * (180 / PI);//将弧度转换成角度
						SetActorRotation(Rot);

						ProjectileMoement->SetVelocityInLocalSpace(FVector(1.0f, 0.0f, 0.0f) * ProjectileMoement->InitialSpeed);//重新设置速度，开启动态模拟
						break;
					}
					case EBulletType::BULLET_RANGE://范围伤害
						ProjectileMoement->StopMovementImmediately();//移动立即停止
						RadialDamage(GetActorLocation(), GetInstigator<ARuleOfTheCharacter>());
						break;
					case EBulletType::BULLET_CHAIN://链式子弹
					{
						ProjectileMoement->StopMovementImmediately();
						BoxDamage->SetCollisionEnabled(ECollisionEnabled::NoCollision);//设置没有任何碰撞,因为通过其他点对点的方式造成伤害，没有必要通过碰撞造成伤害
						UGameplayStatics::SpawnEmitterAttached(DamgeParticle, TargetCharacter->GetHomingPoint());//生成特效，只不过链式特效需要绑定到敌人身上的追踪点
						break;
					}
				}
			}
		}
	}

	//绑定代理
	BoxDamage->OnComponentBeginOverlap.AddUniqueDynamic(this, &ARuleOfTheBullet::OnBeginOverlap);
}

// Called every frame
void ARuleOfTheBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//拿到施法者
	if (ARuleOfTheCharacter* InstigatorCharacter = GetInstigator<ARuleOfTheCharacter>())
	{
		//拿到Controller
		if (ARuleOfTheAIController* InstigatroController = Cast<ARuleOfTheAIController>(InstigatorCharacter->GetController()))
		{
			//拿到Target
			if (ARuleOfTheCharacter* TargetCharacter = InstigatroController->Target.Get())
			{
				//处理不同类型子弹每帧的逻辑
				switch (BulletType)
				{
					case EBulletType::BULLET_CHAIN://链式子弹,每帧显示特效或造成伤害
					{

						TArray<USceneComponent*> SceneComponents;
						RootComponent->GetChildrenComponents(true, SceneComponents);
						for (auto& Temp : SceneComponents)
						{
							if (UParticleSystemComponent* ParticleSystem = Cast<UParticleSystemComponent>(Temp))
							{
								//设置粒子目标点
								ParticleSystem->SetBeamEndPoint(0, TargetCharacter->GetHomingPoint()->GetComponentLocation());
								//设置粒子源点
								ParticleSystem->SetBeamSourcePoint(0, InstigatorCharacter->GetOpenFirePoint()->GetComponentLocation(), 0);
							}
						}
						break;
					}
					case EBulletType::BULLET_TRACK_BEZIER://贝赛尔曲线跟踪类型
					{
						if (Spline)
						{
							FVector DistanceVector = TargetCharacter->GetActorLocation() - InstigatorCharacter->GetActorLocation();
							CurrentSplineTime += DeltaTime;
							float Distance = Spline->GetSplineLength() * (CurrentSplineTime / (DistanceVector.Size() / 1000.0f));//1000是速度,表示1000个像素/帧
							FVector Location = Spline->GetWorldLocationAtDistanceAlongSpline(Distance);
							FRotator Rotator = Spline->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local);
							SetActorLocationAndRotation(Location, Rotator);//设置当前角色子弹的坐标和方向
							//当距离达到最小值时
							if ((Location - TargetCharacter->GetActorLocation()).Size() <= 100.0f)
							{
								//因为这种子弹类型关闭了物理模拟碰撞，所以需要使用射线检测来手动激活碰撞函数
								FHitResult SweepResult;
								SweepResult.Location = Location;
								OnBeginOverlap(nullptr, TargetCharacter, nullptr, 0, false, SweepResult);
							}
						}
						break;
					}
				}

				if (!TargetCharacter->IsActive())
				{
					Destroy();
				}
			}
			else
			{
				Destroy();
			}
		}
	}
}

void ARuleOfTheBullet::RadialDamage(const FVector& Origin, ARuleOfTheCharacter* InstigatorCharacter)
{
	//获取施法者
	if (InstigatorCharacter)
	{
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
		UGameplayStatics::ApplyRadialDamageWithFalloff(
			GetWorld(), 
			100.0f, 10.0f, 
			Origin, 
			400.0f, 1000.0f, 1.0f, 
			UDamageType::StaticClass(), 
			IgnoreActors, 
			GetInstigator());
	}
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

					//处理不同子弹类型碰撞的逻辑
					switch (BulletType)
					{
						case EBulletType::BULLET_DIRECT_LINE:
						case EBulletType::BULLET_LINE:
						case EBulletType::BULLET_TRACK_LINE:
						case EBulletType::BULLET_TRACK_BEZIER:
						{
							//造成伤害
							UGameplayStatics::ApplyDamage(
								OtherCharacter, 
								100.f, 
								InstigatorCharacter->GetController(), 
								InstigatorCharacter,
								UDamageType::StaticClass());
							//销毁
							Destroy();
							break;
						}
					}
				}
			}
		}
	}
}

