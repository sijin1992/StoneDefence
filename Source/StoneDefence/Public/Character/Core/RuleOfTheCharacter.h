// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/Character/IRuleCharacter.h"
#include "Core/GameCore/TowerDefencePlayerController.h"
#include "Core/GameCore/TowerDefenceGameState.h"
#include "RuleOfTheCharacter.generated.h"

UCLASS()
class STONEDEFENCE_API ARuleOfTheCharacter : public ACharacter, public IRuleCharacter
{
	GENERATED_BODY()
	//跟踪点
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttrubute", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* HomingPoint;
	//显示角色信息组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttrubute", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* Widget;
	//开火点
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttrubute", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* OpenFirePoint;
	//碰撞盒
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttrubute", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TraceShowCharacterInformation;

public:
	// Sets default values for this character's properties
	ARuleOfTheCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual bool IsDeath();

	virtual float GetHealth();

	virtual float GetMaxHealth();

	virtual bool IsTeam();

public:
	FORCEINLINE ATowerDefencePlayerController* GetGameController() { return GetWorld() ? (GetWorld()->GetFirstPlayerController<ATowerDefencePlayerController>()) : NULL; }
	FORCEINLINE ATowerDefenceGameState* GetGameState() { return GetWorld() ? (GetWorld()->GetGameState<ATowerDefenceGameState>()) : NULL; }

	FORCEINLINE USceneComponent* GetHomingPoint() const { return HomingPoint; }
	FORCEINLINE UArrowComponent* GetOpenFirePoint() const { return OpenFirePoint; }
};
