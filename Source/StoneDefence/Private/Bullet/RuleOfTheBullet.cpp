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
#include "../StoneDefenceUtils.h"
#include "Data/SkillData.h"
#include "Character/Damage/RuleOfTheDamage.h"

//关闭优化
#if PLATFORM_WINDOWS
#pragma optimize("", off)
#endif

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
	
	SkillID = INDEX_NONE;
	SubmissionSkillRequestType = ESubmissionSkillRequestType::AUTO;//默认技能提交类型为自动
	BulletType = EBulletType::BULLET_DIRECT_LINE;//设置默认子弹类型
	InitialLifeSpan = 4.0f;//4S后自动销毁

	SplineOffset = 0.0f;
	Spline = nullptr;
	CurrentSplineTime = 0.0f;
	ChainAttackCount = 3;
}

// Called when the game starts or when spawned
void ARuleOfTheBullet::BeginPlay()
{
	Super::BeginPlay();

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

const FSkillData* ARuleOfTheBullet::GetSkillData()
{
	if (ATowerDefenceGameState* InGameState = GetWorld()->GetGameState<ATowerDefenceGameState>())
	{
		return InGameState->GetSkillData(SkillID);
	}
	return nullptr;
}

void ARuleOfTheBullet::InitSkill()
{
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
					BoxDamage->SetCollisionEnabled(ECollisionEnabled::NoCollision);//设置没有任何碰撞,因为通过其他点对点的方式造成伤害，没有必要通过碰撞造成伤害
					RadialDamage(GetActorLocation(), GetInstigator<ARuleOfTheCharacter>());
					break;
				case EBulletType::BULLET_CHAIN://链式子弹
				{
					ProjectileMoement->StopMovementImmediately();
					BoxDamage->SetCollisionEnabled(ECollisionEnabled::NoCollision);//设置没有任何碰撞,因为通过其他点对点的方式造成伤害，没有必要通过碰撞造成伤害
					UGameplayStatics::SpawnEmitterAttached(DamgeParticle, TargetCharacter->GetHomingPoint());//生成特效，只不过链式特效需要绑定到敌人身上的追踪点

					//绑定计时器函数
					GetWorld()->GetTimerManager().SetTimer(ChainAttackHandle, this, &ARuleOfTheBullet::ChainAttack, 0.1f);
					//将链式子弹提交变为技能
					SubmissionSkillRequest();
					break;
				}
				case  EBulletType::BULLET_NONE:
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OpenFireParticle, GetActorLocation());
					ProjectileMoement->StopMovementImmediately();//移动立即停止
					BoxDamage->SetCollisionEnabled(ECollisionEnabled::NoCollision);//关闭碰撞

					SubmissionSkillRequest();
					break;
				}
				}
			}
		}
	}
}

void ARuleOfTheBullet::SubmissionSkillRequest()
{
	if (SkillID != INDEX_NONE)
	{
		if (ARuleOfTheCharacter* InstigatorCharacter = Cast<ARuleOfTheCharacter>(GetInstigator()))
		{
			if (ATowerDefenceGameState* InGameState = GetWorld()->GetGameState<ATowerDefenceGameState>())
			{
				const FCharacterData& CharacterData = InstigatorCharacter->GetCharacterData();
				if (CharacterData.IsValid())
				{
					//在客户端验证
					if (!InGameState->IsVerificationSkillTemplate(CharacterData, SkillID))
					{
						//客户端将请求提交到服务器
						InGameState->AddSkillDataTemplateToCharacterData(InstigatorCharacter->GUID, SkillID);

						if (SubmissionSkillRequestType == ESubmissionSkillRequestType::MANUAL)
						{
							//设置技能提交类型,这时发生在服务器上
							InGameState->SetSkillSubmissionType(InstigatorCharacter->GUID, SkillID, SubmissionSkillRequestType);
						}
					}
				}
			}
		}
	}
}

//void ARuleOfTheBullet::ResetIteration()
//{
//	BulletType = EBulletType::BULLET_NONE;
//	InitSkill();
//}

void ARuleOfTheBullet::RadialDamage(const FVector& Origin, ARuleOfTheCharacter* InstigatorCharacter)
{
	//获取施法者
	if (InstigatorCharacter)
	{
		//拿到技能数据
		if (const FSkillData* InSkillData = GetSkillData())
		{
			auto SpawnEffect = [&](ARuleOfTheCharacter* NewCharacter) 
				{
					//生成伤害特效
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DamgeParticle, NewCharacter->GetActorLocation());
					//提交技能请求
					SubmissionSkillRequest();
				};

			TArray<AActor*> IgnoreActors;//忽略/友军列表
			//TArray<AActor*> TargetActors;//敌人列表
			//迭代器遍历忽略的Actor和目标Actor
			for (TActorIterator<ARuleOfTheCharacter>it(GetWorld(), ARuleOfTheCharacter::StaticClass()); it; ++it)
			{
				if (ARuleOfTheCharacter* TheCharacter = *it)
				{
					FVector VDistance = TheCharacter->GetActorLocation() - InstigatorCharacter->GetActorLocation();
					if (VDistance.Size() <= InSkillData->AttackRange)
					{
						if (InSkillData->SkillType.SkillTargetType == ESkillTargetType::FRIENDLY_FORCES)
						{
							if (TheCharacter->GetTeamType() == InstigatorCharacter->GetTeamType())
							{
								SpawnEffect(TheCharacter);
							}
							else
							{
								IgnoreActors.Add(TheCharacter);
							}
						}
						else if (InSkillData->SkillType.SkillTargetType == ESkillTargetType::ENEMY)
						{
							if (TheCharacter->GetTeamType() != InstigatorCharacter->GetTeamType())
							{
								SpawnEffect(TheCharacter);
							}
							else
							{
								IgnoreActors.Add(TheCharacter);
							}
						}
					}
				}
			}

			UClass* RuleOfTheDamage = URuleOfTheDamage::StaticClass();
			if (URuleOfTheDamage* DamageClass = RuleOfTheDamage->GetDefaultObject<URuleOfTheDamage>())//获取CDO单例
			{
				DamageClass->SkillData = InSkillData;
				//造成范围伤害,向四周衰减
				UGameplayStatics::ApplyRadialDamageWithFalloff(
					GetWorld(),
					100.0f, 10.0f,
					Origin,
					400.0f, 1000.0f, 1.0f,
					RuleOfTheDamage,
					IgnoreActors,
					GetInstigator(),
					GetInstigator()->GetController(),
					ECollisionChannel::ECC_MAX);
			}
		}
	}
}

void ARuleOfTheBullet::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//拿到技能数据
	if (const FSkillData* InSkillData = GetSkillData())
	{
		//获取施法者
		if (ARuleOfTheCharacter* InstigatorCharacter = GetInstigator<ARuleOfTheCharacter>())
		{
			//获取碰撞者
			if (ARuleOfTheCharacter* OtherCharacter = Cast<ARuleOfTheCharacter>(Other))
			{
				//验证一致性
				auto VerifyConsistency = [&]()
					{
						bool bConsistency = false;
						if (InSkillData->SkillType.SkillTargetType == ESkillTargetType::FRIENDLY_FORCES)
						{
							bConsistency = InstigatorCharacter->GetTeamType() == OtherCharacter->GetTeamType();
						}
						else if(InSkillData->SkillType.SkillTargetType == ESkillTargetType::ENEMY)
						{
							bConsistency = InstigatorCharacter->GetTeamType() != OtherCharacter->GetTeamType();
						}
						return bConsistency;
					};

				//判断是不是技能目标是不是一致
				if (VerifyConsistency())
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
							UClass* RuleOfTheDamage = URuleOfTheDamage::StaticClass();
							if (URuleOfTheDamage* DamageClass = RuleOfTheDamage->GetDefaultObject<URuleOfTheDamage>())//获取CDO单例
							{
								DamageClass->SkillData = InSkillData;

								//造成伤害,伤害计算在RuleOfTheCharacter里进行计算,这里的100没有用
								UGameplayStatics::ApplyDamage(
									OtherCharacter,
									100.0f,
									InstigatorCharacter->GetController(),
									InstigatorCharacter,
									RuleOfTheDamage);

								//提交技能到数据库
								SubmissionSkillRequest();
							}

							//销毁
							Destroy();
							break;
						}
						case EBulletType::BULLET_RANGE_LINE:
						{
							RadialDamage(OtherCharacter->GetActorLocation(), InstigatorCharacter);
							Destroy();
							break;
						}
						}
					}
				}
			}
		}
	}
}

void ARuleOfTheBullet::ChainAttack()
{
	if (ChainAttackHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(ChainAttackHandle);
	} 

	//主要伤害区
	if (const FSkillData* InSkillData = GetSkillData())
	{
		//拿到施法者
		if (ARuleOfTheCharacter* InstigatorCharacter = GetInstigator<ARuleOfTheCharacter>())
		{
			//拿到Controller
			if (ARuleOfTheAIController* InstigatroController = Cast<ARuleOfTheAIController>(InstigatorCharacter->GetController()))
			{
				//拿到Target
				if (ARuleOfTheCharacter* TargetCharacter = InstigatroController->Target.Get())
				{
					UGameplayStatics::SpawnEmitterAttached(DamgeParticle, TargetCharacter->GetHomingPoint());
					UGameplayStatics::SpawnEmitterAttached(DamgeParticle, InstigatorCharacter->GetHomingPoint());

					UClass* RuleOfTheDamage = URuleOfTheDamage::StaticClass();
					if (URuleOfTheDamage* DamageClass = RuleOfTheDamage->GetDefaultObject<URuleOfTheDamage>())//获取CDO单例
					{
						DamageClass->SkillData = InSkillData;

						UGameplayStatics::ApplyDamage(
							TargetCharacter,
							100.0f,
							InstigatorCharacter->GetController(),
							InstigatorCharacter,
							RuleOfTheDamage);
					}
				}
			}
		}
	}

	ChainAttackCount--;
	if (ChainAttackCount > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(ChainAttackHandle, this, &ARuleOfTheBullet::ChainAttack, 0.3f);
	}
}

//打开优化
#if PLATFORM_WINDOWS
#pragma optimize("", on)
#endif

