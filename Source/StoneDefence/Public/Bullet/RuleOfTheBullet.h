// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../StoneDefenceType.h"
#include "Character/Core/RuleOfTheCharacter.h"
#include "RuleOfTheBullet.generated.h"

class USplineComponent;
UCLASS()
class STONEDEFENCE_API ARuleOfTheBullet : public AActor
{
	GENERATED_BODY()
	//根组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttrubute", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootBullet;
	//碰撞盒子
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttrubute", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* BoxDamage;
	//具有移动属性的组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttrubute", meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMoement;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Bullet")
	TEnumAsByte<EBulletType> BulletType;
	//开火特效
	UPROPERTY(EditDefaultsOnly, Category = "Bullet")
	UParticleSystem* OpenFireParticle;
	//子弹伤害特效(碰撞后产生的特效)
	UPROPERTY(EditDefaultsOnly,Category = "Bullet")
	UParticleSystem* DamgeParticle;

	//贝塞尔曲线轨迹的跟踪子弹Z轴方向偏移量
	UPROPERTY(EditDefaultsOnly, Category = "Bullet Track Bezier")
	float SplineOffset;

public:	
	// Sets default values for this actor's properties
	ARuleOfTheBullet();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//范围伤害
	void RadialDamage(const FVector& Origin, ARuleOfTheCharacter* InstigatorCharacter);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY()
	USplineComponent *Spline;

	float CurrentSplineTime;
};
