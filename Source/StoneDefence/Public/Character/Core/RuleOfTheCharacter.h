// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/Character/RuleCharacter.h"
#include "Core/GameCore/TowerDefencePlayerController.h"
#include "Core/GameCore/TowerDefenceGameState.h"
#include "../StoneDefenceType.h"
#include "Core/GameCore/TowerDefencePlayerState.h"
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
	//角色ID
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterAttribute")
	FGuid GUID;

	//角色类型
	UPROPERTY(EditDefaultsOnly, Category = "CharacterType")
	TEnumAsByte<EGameCharacterType::Type> CharacterType;

	//是否在攻击
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterAttribute")
	bool bAttack;
	//飘字
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class ADrawText> DrawTextClass;

	UPROPERTY(EditDefaultsOnly, Category = Death)
	float DelayDeath;
//主要是防止编辑器崩溃
#if WITH_EDITOR
private:
	FCharacterData NULLCharacterData;
#endif

public:	
	// Sets default values for this character's properties
	ARuleOfTheCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UpdateUI();

	UFUNCTION()
	void ResetGUID();

	virtual EGameCharacterType::Type GetCharacterType();

	virtual bool IsDeath();

	UFUNCTION(Blueprintable, BlueprintPure, Category = "Towers|Attrubute")
	bool IsActive() { return !IsDeath(); }

	virtual float GetHealth();

	virtual float GetMaxHealth();
	//注册队伍类型(由服务器调用)
	virtual void RegisterTeam() {};

	virtual ETeam GetTeamType();

	virtual FCharacterData& GetCharacterData();
	//获取静态模型组件
	UFUNCTION(Blueprintable, BlueprintPure, Category = "Towers|Test")
	UStaticMesh* GetDollMesh(FTransform& InTransform);
	//通知蓝图攻击开始
	UFUNCTION(BlueprintImplementableEvent)
	void AttackBegin();
	//通知蓝图攻击结束
	UFUNCTION(BlueprintImplementableEvent)
	void AttackEnd();
	//通知蓝图角色死亡了
	UFUNCTION(BlueprintImplementableEvent)
	void CharacterDeath();
	//通知蓝图动画标签
	UFUNCTION(BlueprintImplementableEvent)
	void AnimTag();

	//如果是网络游戏的话需要加：UFUNCTION(Client)，代表这个逻辑是执行在客户端的
	UFUNCTION(/*Client*/)
	void AddSkillSlot_Client(const FGuid& SlotID);//添加技能UI
	UFUNCTION(/*Client*/)
	void RemoveSkillSlot_Client(const FGuid& SlotID);//移除技能UI

public:
	FORCEINLINE ATowerDefencePlayerController* GetGameController() { return GetWorld() ? (GetWorld()->GetFirstPlayerController<ATowerDefencePlayerController>()) : NULL; }
	FORCEINLINE ATowerDefenceGameState* GetGameState() { return GetWorld() ? (GetWorld()->GetGameState<ATowerDefenceGameState>()) : nullptr; }
	FORCEINLINE ATowerDefencePlayerState* GetPlayerState() { return GetWorld() ? (GetGameController()->GetPlayerState<ATowerDefencePlayerState>()) : nullptr; }

	FORCEINLINE USceneComponent* GetHomingPoint() const { return HomingPoint; }
	FORCEINLINE UArrowComponent* GetOpenFirePoint() const { return OpenFirePoint; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION()
	virtual void OnClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);
};
