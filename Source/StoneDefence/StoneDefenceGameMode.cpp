// Copyright Epic Games, Inc. All Rights Reserved.


#include "StoneDefenceGameMode.h"
#include "Core/GameCore/TowerDefenceGameState.h"
#include "Core/GameCore/TowerDefencePlayerController.h"
#include "Core/GameCore/TowerDefenceGameCamrea.h"
#include "Core/GameCore/TowerDefencePlayerState.h"
#include "UI/GameUI/Core/RuleOfTheHUD.h"
#include "StoneDefenceUtils.h"
#include "Character/Core/RuleOfTheCharacter.h"
#include "Items/SpawnPoint.h"
#include "Engine/DataTable.h"
#include "Character/CharacterCore/Towers.h"
#include "Character/CharacterCore/Monsters.h"
#include "Engine/World.h"

//关闭优化
#if PLATFORM_WINDOWS
#pragma optimize("", off)
#endif

AStoneDefenceGameMode::AStoneDefenceGameMode()
{
	//加载GameState
	GameStateClass = ATowerDefenceGameState::StaticClass();
	//加载Controller
	PlayerControllerClass = ATowerDefencePlayerController::StaticClass();
	//加载ControllerPawn
	DefaultPawnClass = ATowerDefenceGameCamrea::StaticClass();
	//加载PlayerState
	PlayerStateClass = ATowerDefencePlayerState::StaticClass();
	//加载HUD
	HUDClass = ARuleOfTheHUD::StaticClass();
}

void AStoneDefenceGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Game BeginPlay"));

	if (ATowerDefenceGameState* InGameState = GetGameState<ATowerDefenceGameState>())
	{
		InGameState->GetGameData().AssignedMonsterAmount();//计算每一波的怪物数量
	}
	//生成主塔
	SpawnMainTowerRule();
}

void AStoneDefenceGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (ATowerDefenceGameState* InGameState = GetGameState<ATowerDefenceGameState>())
	{
		//通知更新客户端
		CallUpdateAllClient(
			[&](ATowerDefencePlayerController* MyPlayerController)
			{
				if (ATowerDefencePlayerState* InPlayerState = MyPlayerController->GetPlayerState<ATowerDefencePlayerState>())
				{
					InPlayerState->GetPlayerData().GameGoldTime += DeltaSeconds;
					if (InPlayerState->GetPlayerData().IsAllowIncrease())
					{
						InPlayerState->GetPlayerData().GameGoldTime = 0.0f;
						InPlayerState->GetPlayerData().GameGold++;
					}
				}
			}
		);

		if (InGameState->GetGameData().GameCount <= 0.0f)
		{
			InGameState->GetGameData().bGameOver = true;
		}
		else
		{
			InGameState->GetGameData().GameCount -= DeltaSeconds;
		}

		int32 TowersNum = 0;
		TArray<ARuleOfTheCharacter*> InTowers;
		StoneDefenceUtils::GetAllActor<ATowers>(GetWorld(), InTowers);
		for (ARuleOfTheCharacter* Tower : InTowers)
		{
			if (Tower->IsActive())
			{
				TowersNum++;
			}
		}

		if (TowersNum == 0)
		{
			InGameState->GetGameData().bGameOver = true;
		}
	}

	//生成怪物
	SpawnMonstersRule(DeltaSeconds);

	//更新技能
	UpdateSkill(DeltaSeconds);
}

void AStoneDefenceGameMode::SpawnMainTowerRule()
{
	for (ASpawnPoint* TargetPoint : StoneDefenceUtils::GetAllActor<ASpawnPoint>(GetWorld()))
	{
		if (TargetPoint->Team == ETeam::RED)
		{
			SpawnTower(0, 1, TargetPoint->GetActorLocation(), TargetPoint->GetActorRotation());
		}
	}
}

//获取难度系数
int32 GetMonsterLevel(UWorld* InWorld)
{
	//难度系数结构体
	struct FDifficultyDetermination
	{
		FDifficultyDetermination()
			:Level(0),
			Combination(0.0f),
			Attack(0.0f),
			Defence(0.0f),
			Variance(0.0f)
		{
		}

		float Level;//等级
		float Combination;//配合度
		float Attack;//攻击系数
		float Defence;//防御系数
		float Variance;//当前随机值的离散度/方差
	};

	auto  GetDifficultyDetermination = [](TArray<ARuleOfTheCharacter*>& RuleOfTheCharacter) ->FDifficultyDetermination
		{
			FDifficultyDetermination DifficultyDetermination;
			int32 Index = 0;
			//遍历所有的塔或怪物
			for (ARuleOfTheCharacter* Temp : RuleOfTheCharacter)
			{
				if (Temp->IsActive())//如果塔还活着
				{
					//拿到等级
					DifficultyDetermination.Level += (float)Temp->GetCharacterData().Lv;
					DifficultyDetermination.Attack += Temp->GetCharacterData().GetAttack();
					DifficultyDetermination.Defence += Temp->GetCharacterData().GetArmor();
					Index++;
				}
			}

			DifficultyDetermination.Level /= Index;//平均值代表怪物的起始等级
			DifficultyDetermination.Attack /= Index;
			DifficultyDetermination.Defence /= Index;
			//求方差
			for (ARuleOfTheCharacter* Temp : RuleOfTheCharacter)
			{
				if (Temp->IsActive())//如果塔还活着
				{
					//计算方差
					float InVaue = (float)Temp->GetCharacterData().Lv - DifficultyDetermination.Level;
					DifficultyDetermination.Variance += InVaue * InVaue;
				}
			}

			DifficultyDetermination.Variance /= Index;

			return DifficultyDetermination;
		};

	TArray<ARuleOfTheCharacter*> Towers;
	StoneDefenceUtils::GetAllActor<ATowers, ARuleOfTheCharacter>(InWorld, Towers);
	FDifficultyDetermination TowerDD = GetDifficultyDetermination(Towers);
	TArray<ARuleOfTheCharacter*> Monsters;
	StoneDefenceUtils::GetAllActor<AMonsters, ARuleOfTheCharacter>(InWorld, Monsters);
	FDifficultyDetermination MonsterDD = GetDifficultyDetermination(Monsters);

	int32 ReturnLevel = (int32)TowerDD.Level;
	if (TowerDD.Attack > MonsterDD.Attack)
	{
		ReturnLevel++;
	}
	if (TowerDD.Defence > MonsterDD.Defence)
	{
		ReturnLevel++;
	}

	//ReturnLevel += FMath::Abs(2 - FMath::Sqrt(TowerDD.Variance / 10.0f));//2是保护值,对方差开根

	return ReturnLevel;
}

void AStoneDefenceGameMode::SpawnMonstersRule(float DeltaSeconds)
{
	if (ATowerDefenceGameState* InGameState = GetGameState<ATowerDefenceGameState>())
	{
		//判断当前关卡是否胜利
		if (!InGameState->GetGameData().bCurrentLevelMissionSuccess)
		{
			//是否输掉比赛
			if (!InGameState->GetGameData().bGameOver)
			{
				//怪物数量是否为0
				if (InGameState->GetGameData().PerNumberOfMonsters.Num())
				{
					InGameState->GetGameData().CurrentSpawnMonsterTime += DeltaSeconds;
					if (InGameState->GetGameData().IsAllowSpawnMonster())
					{
						InGameState->GetGameData().ResetSpawnMonsterTime();

						int32 MonsterLevel = GetMonsterLevel(GetWorld());
						if (ARuleOfTheCharacter* MyMonster = SpawnMonster(0, MonsterLevel, FVector::ZeroVector, FRotator::ZeroRotator))
						{
							TArray<ASpawnPoint*> MonsterSpawnPoints;
							for (ASpawnPoint* TargetPoint : StoneDefenceUtils::GetAllActor<ASpawnPoint>(GetWorld()))
							{
								if (MyMonster->GetTeamType() == TargetPoint->Team)
								{
									MonsterSpawnPoints.Add(TargetPoint);
									break;
								}
							}
							ASpawnPoint* TargetPoint = MonsterSpawnPoints[FMath::RandRange(0, MonsterSpawnPoints.Num() - 1)];
							MyMonster->SetActorLocationAndRotation(TargetPoint->GetActorLocation(), TargetPoint->GetActorRotation());
							//计算剩余阶段
							InGameState->GetGameData().StageDecision();
						}
					}
				}
			}
		}
		else
		{

		}
	}
}

ATowers* AStoneDefenceGameMode::SpawnTower(const int32 CharacterID, int32 CharacterLevel, const FVector& Location, const FRotator& Rotator)
{
	return SpawnCharacter<ATowers>(CharacterID, CharacterLevel, GetGameState<ATowerDefenceGameState>()->AITowerCharacterData, Location, Rotator);
}

AMonsters* AStoneDefenceGameMode::SpawnMonster(const int32 CharacterID, int32 CharacterLevel, const FVector& Location, const FRotator& Rotator)
{
	return SpawnCharacter<AMonsters>(CharacterID, CharacterLevel, GetGameState<ATowerDefenceGameState>()->AIMonsterCharacterData, Location, Rotator);
}

ARuleOfTheCharacter* AStoneDefenceGameMode::SpawnCharacter(
	const int32 CharacterID,
	int32 CharacterLevel,
	const UDataTable* InCharacterData,
	const FVector& Location,
	const FRotator& Rotator)
{
	ARuleOfTheCharacter* OutCharacter = nullptr;
	if (ATowerDefenceGameState* InGameState = GetGameState<ATowerDefenceGameState>())
	{
		if (InCharacterData)
		{
			TArray<FCharacterData*> Datas;
			InCharacterData->GetAllRows(TEXT("Character Data"), Datas);

			auto GetCharacterData = [&](int32 ID) ->const FCharacterData*
				{
					for (auto& Temp : Datas)
					{
						if (Temp->ID == ID)
						{
							return Temp;
						}
					}
					return nullptr;
				};


			if (const FCharacterData* CharacterData = GetCharacterData(CharacterID))
			{
				UClass* NewClass = CharacterData->CharacterBlueprintKey.LoadSynchronous();//LoadSynchronous读取
				if (GetWorld() && NewClass)
				{
					if (ARuleOfTheCharacter* RuleOfTheCharacter = GetWorld()->SpawnActor<ARuleOfTheCharacter>(NewClass, Location, Rotator))
					{
						FCharacterData& CharacterInstance = InGameState->AddCharacterData(RuleOfTheCharacter->GUID, *CharacterData);
						CharacterInstance.UpdateHealth();
						if (CharacterLevel > 1)
						{
							//升极
							for (int32 i = 0; i < CharacterLevel; i++)
							{
								CharacterInstance.UpdateLevel();
							}
						}
						//初始化技能数据
						InGameState->InitSkill(CharacterInstance);

						//注册队伍类型
						RuleOfTheCharacter->RegisterTeam();
						OutCharacter = RuleOfTheCharacter;
					}
				}
			}
		}
	}
	return OutCharacter;
}

void AStoneDefenceGameMode::UpdateSkill(float DeltaSeconds)
{
	if (ATowerDefenceGameState* InGameState = GetGameState<ATowerDefenceGameState>())
	{

		//将相同阵营的距离最近的角色加入数组
		auto AddRecentCharacterToArray = [&](TArray<FCharacterData*>& TeamArray, const TPair<FGuid, FCharacterData>& InSpellcaster, FCharacterData& InOther, float InRange)
			{
				//是否无限距离
				if (InRange != 0)
				{
					float Distance = (InSpellcaster.Value.Location - InOther.Location).Size();
					if (Distance <= InRange)
					{
						TeamArray.Add(&InOther);
					}
				}
				else
				{
					//场景中所有的角色都加进去
					TeamArray.Add(&InOther);
				}
			};


		//获取距离最近的角色列表
		auto GetTeam = [&](TArray<FCharacterData*>& TeamArray, const TPair<FGuid, FCharacterData>& InSpellcaster, float InRange, bool bReversed = false)
			{
				for (auto& Temp : InGameState->GetSaveData()->CharacterDatas)
				{
					if (bReversed)
					{
						if (Temp.Value.Team != InSpellcaster.Value.Team)//寻找敌人列表
						{
							AddRecentCharacterToArray(TeamArray, InSpellcaster, Temp.Value, InRange);
						}
					}
					else
					{
						if (Temp.Value.Team == InSpellcaster.Value.Team)//寻找友军列表
						{
							AddRecentCharacterToArray(TeamArray, InSpellcaster, Temp.Value, InRange);
						}
					}
				}
			};

		//判断角色身上是否已经拥有某个技能
		auto IsVerificationSkill = [](const FCharacterData& InCharacterData, int32 SkillID)->bool
			{
				for (auto& InSkill : InCharacterData.AdditionalSkillData)
				{
					if (InSkill.Value.ID == SkillID)
					{
						return true;
					}
				}
				return false;
			};

		//给单个角色挂上技能
		auto AddSkill = [&](FCharacterData* InCharacterData, FSkillData& InSkill)
			{
				if (!IsVerificationSkill(*InCharacterData, InSkill.ID))
				{
					FGuid NewSkillID = FGuid::NewGuid();
					InCharacterData->AdditionalSkillData.Add(NewSkillID, InSkill);
					//通知客户端更新添加UI
					CallUpdateAllClient([&](ATowerDefencePlayerController* MyPlayerController)
						{
							MyPlayerController->AddSkillSlot_Client(NewSkillID);
						});
				}
			};

		//给某个队伍全员挂上技能
		auto AddSkillToForces = [&](TArray<FCharacterData*>& InForces, FSkillData& InSkill)
			{
				for (auto& CharacterElement : InForces)
				{
					//如果角色身上没有这个技能，就给他挂上这个技能
					if (!IsVerificationSkill(*CharacterElement, InSkill.ID))
					{
						FGuid NewSkillID = FGuid::NewGuid();
						//TODO代理
						CharacterElement->AdditionalSkillData.Add(NewSkillID, InSkill);
					}
				}
			};

		//寻找最近的单个目标
		auto FindRangeTargetRecently = [&](const TPair<FGuid, FCharacterData>& InSpellcaster, bool bReversed = false) ->FCharacterData*
			{
				float TargetDistance = 99999999;
				FGuid InFGuid;

				auto InitTargetRecently = [&](TPair<FGuid, FCharacterData>& Pair)
					{
						FVector Location = Pair.Value.Location;
						FVector TempVector = Location - InSpellcaster.Value.Location;
						float Distance = TempVector.Size();

						if (Distance < TargetDistance && Pair.Value.Health > 0)
						{
							InFGuid = Pair.Key;
							TargetDistance = Distance;
						}
					};

				for (auto& Temp : InGameState->GetSaveData()->CharacterDatas)
				{
					if (InSpellcaster.Key != Temp.Key)//排除施法者自己
					{
						if (bReversed)
						{
							//寻找敌人
							if (InSpellcaster.Value.Team != Temp.Value.Team)
							{
								InitTargetRecently(Temp);
							}
						}
						else
						{
							//寻找友军
							if (InSpellcaster.Value.Team == Temp.Value.Team)
							{
								InitTargetRecently(Temp);
							}
						}
					}
				}

				if (InFGuid != FGuid())
				{
					for (auto &CharacterTemp : InGameState->GetSaveData()->CharacterDatas)
					{
						if (CharacterTemp.Key == InFGuid)
						{
							return &CharacterTemp.Value;
						}
					}
				}
				return nullptr;
			};
		
		//获取数据表中的技能基础模板
		const TArray<FSkillData*>&SkillDataTemplate = InGameState->GetSkillDataFromTable();

		//遍历场景中所有角色的技能,先清理需要移除的技能，然后再释放或添加技能
		for (auto& InSpellcaster : InGameState->GetSaveData()->CharacterDatas)
		{
			//遍历查找需要移除的技能
			TArray<FGuid> RemoveSkill;
			for (auto& SkillTemp : InSpellcaster.Value.AdditionalSkillData)
			{

				//爆发类型的技能只存在1帧，所以可以释放后立即移除
				if (SkillTemp.Value.SkillType.SkillType == ESkillType::BURST)
				{
					RemoveSkill.Add(SkillTemp.Key);
				}


				//限时技能和迭代技能时间到了自然移除
				if (SkillTemp.Value.SkillType.SkillType == ESkillType::SECTION ||
					SkillTemp.Value.SkillType.SkillType == ESkillType::ITERATION)
				{
					SkillTemp.Value.SkillDuration += DeltaSeconds;
					if (SkillTemp.Value.SkillDuration >= SkillTemp.Value.MaxSkillDuration)
					{
						RemoveSkill.Add(SkillTemp.Key);
					}
				}

				//迭代技能进行持续更新技能
				if (SkillTemp.Value.SkillType.SkillType == ESkillType::ITERATION)
				{
					SkillTemp.Value.SkillDurationTime += DeltaSeconds;
					if (SkillTemp.Value.SkillDurationTime >= 1.0f)
					{
						SkillTemp.Value.SkillDurationTime = 0.0f;
						//判断是增益还是减益
						if (SkillTemp.Value.SkillType.SkillEffectType == ESkillEffectType::ADD)
						{
							InSpellcaster.Value.Health += SkillTemp.Value.Health;
							InSpellcaster.Value.PhysicalAttack += SkillTemp.Value.PhysicalAttack;
							InSpellcaster.Value.Armor += SkillTemp.Value.Armor;
							InSpellcaster.Value.AttackSpeed += SkillTemp.Value.AttackSpeed;
							InSpellcaster.Value.Gold += SkillTemp.Value.Gold;
						}
						else
						{
							InSpellcaster.Value.Health -= SkillTemp.Value.Health;
							InSpellcaster.Value.PhysicalAttack -= SkillTemp.Value.PhysicalAttack;
							InSpellcaster.Value.Armor -= SkillTemp.Value.Armor;
							InSpellcaster.Value.AttackSpeed -= SkillTemp.Value.AttackSpeed;
							InSpellcaster.Value.Gold -= SkillTemp.Value.Gold;
						}
						
					}
				}

				//通知客户端生成子弹特效
				CallUpdateAllClient([&](ATowerDefencePlayerController* MyPlayerController)
					{
						MyPlayerController->SpawnBullet_Client(InSpellcaster.Key, SkillTemp.Value.BulletClass);
					});
			}

			//移除技能
			for (FGuid& SkillFGuid : RemoveSkill)
			{
				InSpellcaster.Value.AdditionalSkillData.Remove(SkillFGuid);
			}

			//技能的释放
			for (auto& InSkill : InSpellcaster.Value.CharacterSkills)
			{
				InSkill.CDTime += DeltaSeconds;
				if (InSkill.CDTime >= InSkill.CD)
				{
					InSkill.CDTime = 0.0f;
					if (InSkill.SkillType.SkillAttackType == ESkillAttackType::MULTIPLE)//群体技能
					{
						TArray<FCharacterData*> RecentForces;
						if (InSkill.SkillType.SkillTargetType == ESkillTargetType::FRIENDLY_FORCES)//友军技能
						{
							GetTeam(RecentForces, InSpellcaster, InSkill.AttackRange);
						}
						else if (InSkill.SkillType.SkillTargetType == ESkillTargetType::ENEMY)//对敌技能
						{
							GetTeam(RecentForces, InSpellcaster, InSkill.AttackRange, true);
						}
						//挂上技能
						AddSkillToForces(RecentForces, InSkill);
					}
					else if (InSkill.SkillType.SkillAttackType == ESkillAttackType::SINGLE)//单体技能
					{
						FCharacterData* Recent = nullptr;
						if (InSkill.SkillType.SkillTargetType == ESkillTargetType::FRIENDLY_FORCES)//友军技能
						{
							Recent = FindRangeTargetRecently(InSpellcaster);
						}
						else if (InSkill.SkillType.SkillTargetType == ESkillTargetType::ENEMY)//对敌技能
						{
							Recent = FindRangeTargetRecently(InSpellcaster, true);
						}
						//挂上技能
						AddSkill(Recent, InSkill);
					}
				}
			}
		}
	}
}

void AStoneDefenceGameMode::CallUpdateAllClient(TFunction<void(ATowerDefencePlayerController* MyPlayerController)> InImplement)
{
	//这里是因为服务器有多个PlayerController,所以需要遍历多个PlayerController
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		//这里是因为是单机，所以直接转了，如果是网络游戏需要判断是否是要找的PlayerController
		if (ATowerDefencePlayerController* MyPlayerController = Cast<ATowerDefencePlayerController>(It->Get()))
		{
			InImplement(MyPlayerController);
		}
	}
}

//打开优化
#if PLATFORM_WINDOWS
#pragma optimize("", on)
#endif