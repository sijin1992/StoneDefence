// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/Character/RuleCharacter.h"
#include "Core/GameCore/TowerDefencePlayerController.h"
#include "Core/GameCore/TowerDefenceGameState.h"
#include "../StoneDefenceType.h"
#include "RuleOfTheCharacter.generated.h"

UCLASS()
class STONEDEFENCE_API ARuleOfTheCharacter : public ACharacter, public IRuleCharacter
{
	GENERATED_BODY()
	//跟踪点
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterComponent", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* HomingPoint;
	//显示角色信息组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterComponent", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* Widget;
	//开火点
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterComponent", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* OpenFirePoint;
	//碰撞盒
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterComponent", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TraceShowCharacterInformation;
public:
	UPROPERTY(EditDefaultsOnly,Category = UI)
	TSubclassOf<class ADrawText> DrawTextClass;

public:
	//角色ID
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterAttribute")
	FGuid GUID;
	//是否在攻击
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterAttribute")
	bool bAttack;
public:	
	// Sets default values for this character's properties
	ARuleOfTheCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UpdateUI();

	virtual EGameCharacterType::Type GetType();

	virtual bool IsDeath();

	UFUNCTION(Blueprintable, BlueprintPure, Category = "Towers|Attrubute")
	bool IsActive() { return !IsDeath(); }

	virtual float GetHealth();

	virtual float GetMaxHealth();

	virtual bool IsTeam();

	virtual FCharacterData& GetCharacterData();
	//获取静态模型组件
	UFUNCTION(Blueprintable, BlueprintPure, Category = "Towers|Test")
	UStaticMesh* GetDollMesh(FTransform& InTransform);

public:
	FORCEINLINE ATowerDefencePlayerController* GetGameController() { return GetWorld() ? (GetWorld()->GetFirstPlayerController<ATowerDefencePlayerController>()) : NULL; }
	FORCEINLINE ATowerDefenceGameState* GetGameState() { return GetWorld() ? (GetWorld()->GetGameState<ATowerDefenceGameState>()) : nullptr; }

	FORCEINLINE USceneComponent* GetHomingPoint() const { return HomingPoint; }
	FORCEINLINE UArrowComponent* GetOpenFirePoint() const { return OpenFirePoint; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
