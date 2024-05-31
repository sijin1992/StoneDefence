// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Core/RuleOfTheCharacter.h"
#include "Towers.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API ATowers : public ARuleOfTheCharacter
{
	GENERATED_BODY()
	//塔是特效模型
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttrubute", meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* ParticleMesh;
	//静态模型可建造的范围
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttrubute", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMeshBuilding;
	//可破坏组件,塔死亡后将特效模型改成该组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttrubute", meta = (AllowPrivateAccess = "true"))
	class UDestructibleComponent* DestructibleMeshBuilding;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttrubute")
	FRotator TowersRotator;
public:
	ATowers();

	virtual bool IsTeam();

	virtual EGameCharacterType::Type GetType();

protected:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void OnClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

};
