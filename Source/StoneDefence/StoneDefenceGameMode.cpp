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
	
	//更新玩家数据
	UpdatePlayerData(DeltaSeconds);

	//更新游戏场景规则
	UpdateGameData(DeltaSeconds);

	//更新生成怪物
	UpdateMonstersRule(DeltaSeconds);

	//更新技能
	UpdateSkill(DeltaSeconds);

	//更新背包
	UpdateInventory(DeltaSeconds);
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

void AStoneDefenceGameMode::UpdateMonstersRule(float DeltaSeconds)
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

void AStoneDefenceGameMode::UpdatePlayerData(float DeltaSeconds)
{
	if (ATowerDefenceGameState* InGameState = GetGameState<ATowerDefenceGameState>())
	{
		//通知更新客户端
		StoneDefenceUtils::CallUpdateAllClient(
			GetWorld(),
			[&](ATowerDefencePlayerController* MyPlayerController)
			{
				if (ATowerDefencePlayerState* InPlayerState = MyPlayerController->GetPlayerState<ATowerDefencePlayerState>())
				{
					//游戏金币更新
					InPlayerState->GetPlayerData().GameGoldTime += DeltaSeconds;
					if (InPlayerState->GetPlayerData().IsAllowIncrease())
					{
						InPlayerState->GetPlayerData().GameGoldTime = 0.0f;
						InPlayerState->GetPlayerData().GameGold++;
					}
				}
			}
		);
	}
}

void AStoneDefenceGameMode::UpdateGameData(float DeltaSeconds)
{
	if (ATowerDefenceGameState* InGameState = GetGameState<ATowerDefenceGameState>())
	{
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
						RuleOfTheCharacter->ResetGUID();

						FCharacterData& CharacterInstance = InGameState->AddCharacterData(RuleOfTheCharacter->GUID, *CharacterData);
						CharacterInstance.UpdateHealth();
						if (CharacterLevel > 1)
						{
							//升级
							for (int32 i = 0; i < CharacterLevel; i++)
							{
								CharacterInstance.UpdateLevel();
							}
						}
						//初始化角色的被动技能
						RuleOfTheCharacter->InitSkill();

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
		/*
		
		//获取距离最近的角色列表
		auto GetTeam = [&](TArray<TPair<FGuid, FCharacterData>*>& TeamArray, const TPair<FGuid, FCharacterData>& InSpellcaster, float InRange, bool bReversed = false)
			{

				//将相同阵营的距离最近的角色加入数组
				auto AddRecentCharacterToArray = [&](TPair<FGuid, FCharacterData>& InOther)
					{
						//是否无限距离
						if (InRange != 0)
						{
							float Distance = (InOther.Value.Location - InSpellcaster.Value.Location).Size();
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


				for (auto& Temp : InGameState->GetSaveData()->CharacterDatas)
				{
					if (bReversed)
					{
						if (Temp.Value.Team != InSpellcaster.Value.Team)//寻找敌人列表
						{
							AddRecentCharacterToArray(Temp);
						}
					}
					else
					{
						if (Temp.Value.Team == InSpellcaster.Value.Team)//寻找友军列表
						{
							AddRecentCharacterToArray(Temp);
						}
					}
				}
			};

		//给某个队伍全员挂上技能
		auto AddSkillToForces = [&](TArray<TPair<FGuid, FCharacterData>*>& InForces, FSkillData& InSkill)
			{
				for (auto& CharacterElement : InForces)
				{
					InGameState->AddSkill(*CharacterElement, InSkill);
				}
			};

		//寻找最近的单个目标
		auto FindRangeTargetRecently = [&](const TPair<FGuid, FCharacterData>& InSpellcaster, bool bReversed = false) ->TPair<FGuid, FCharacterData>*
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
							return &CharacterTemp;
						}
					}
				}
				return nullptr;
			};

		*/
		
		//获取数据表中的技能基础模板
		const TArray<FSkillData*>&SkillDataTemplate = InGameState->GetSkillDataFromTable();

		//遍历场景中所有角色的技能,先清理需要移除的技能，然后再释放或添加技能
		for (auto& InSpellcaster : InGameState->GetSaveData()->CharacterDatas)
		{
			if (InSpellcaster.Value.Health > 0.0f)
			{
				//遍历查找需要移除的技能
				TArray<FGuid> RemoveSkill;
				for (auto& SkillTemp : InSpellcaster.Value.AdditionalSkillData)
				{
					SkillTemp.Value.SkillDurationTime += DeltaSeconds;
					//爆发类型的技能只存在1帧，所以可以释放后立即移除
					if (SkillTemp.Value.SkillType.SkillType == ESkillType::BURST)
					{
						RemoveSkill.Add(SkillTemp.Key);
					}


					//限时技能和迭代技能时间到了自然移除
					if (SkillTemp.Value.SkillType.SkillType == ESkillType::SECTION ||
						SkillTemp.Value.SkillType.SkillType == ESkillType::ITERATION)
					{
						SkillTemp.Value.SkillDuration -= DeltaSeconds;
						if (SkillTemp.Value.SkillDuration <= 0.0f)
						{
							RemoveSkill.Add(SkillTemp.Key);
						}
					}

					//迭代技能进行持续更新技能
					if (SkillTemp.Value.SkillType.SkillType == ESkillType::ITERATION)
					{
						if (SkillTemp.Value.SkillDurationTime >= 1.0f)
						{
							SkillTemp.Value.SkillDurationTime = 0.0f;

							//更新所有客户端
							StoneDefenceUtils::CallUpdateAllClient(GetWorld(), [&](ATowerDefencePlayerController* MyPlayerController)
								{
									//通知客户端生成子弹特效
									MyPlayerController->SpawnBullet_S2C(InSpellcaster.Key, SkillTemp.Value.ID);
								});

						}
					}
				}

				//移除技能
				for (FGuid& SkillFGuid : RemoveSkill)
				{
					//通知客户端移除技能Icon
					StoneDefenceUtils::CallUpdateAllClient(
						GetWorld(),
						[&](ATowerDefencePlayerController* MyPlayerController)
						{
							MyPlayerController->RemoveSkillSlot_S2C(InSpellcaster.Key, SkillFGuid);
						});
					InSpellcaster.Value.AdditionalSkillData.Remove(SkillFGuid);
				}

				//已经释放过的技能列表
				TArray<FSkillData> RemoveBecomeEffectiveSkills;
				//主技能的释放
				for (auto& InSkill : InSpellcaster.Value.CharacterSkills)
				{
					InSkill.CDTime += DeltaSeconds;
					if (InSkill.CDTime >= InSkill.CD)
					{
						InSkill.CDTime = 0.0f;
						if (!InSkill.bBecomeEffective)
						{
							InSkill.bBecomeEffective = true;
						}
						else
						{
							RemoveBecomeEffectiveSkills.Add(InSkill);
						}
					}
				}
				//移除刚刚释放过的技能
				for (auto& InSkill : RemoveBecomeEffectiveSkills)
				{
					//施法者移除技能
					InSpellcaster.Value.CharacterSkills.Remove(InSkill);
					//判断是否是自动提交技能类型，自动就是服务器提交
					if (InSkill.SubmissionSkillRequestType == ESubmissionSkillRequestType::AUTO)
					{
						//更新所有客户端
						StoneDefenceUtils::CallUpdateAllClient(GetWorld(), [&](ATowerDefencePlayerController* MyPlayerController)
							{
								//通知客户端生成子弹特效
								MyPlayerController->SpawnBullet_S2C(InSpellcaster.Key, InSkill.ID);
							});
					}

				}
			}
		}
	}
}

void AStoneDefenceGameMode::UpdateInventory(float DeltaSeconds)
{
	if (ATowerDefenceGameState* InGameState = GetGameState<ATowerDefenceGameState>())
	{
		StoneDefenceUtils::CallUpdateAllClient(GetWorld(), [&](ATowerDefencePlayerController* MyPlayerController)
			{
				if (ATowerDefencePlayerState* InPlayerState = MyPlayerController->GetPlayerState<ATowerDefencePlayerState>())
				{
					for (auto& BuildingTowerPair : InPlayerState->GetSaveData()->BuildingTowers)
					{
						if (BuildingTowerPair.Value.IsValid())
						{
							if (!BuildingTowerPair.Value.bLockCD)
							{
								if (!BuildingTowerPair.Value.bDragIcon)
								{
									if (BuildingTowerPair.Value.CurrentConstructionTowersCD > 0)
									{
										BuildingTowerPair.Value.CurrentConstructionTowersCD -= DeltaSeconds;
										BuildingTowerPair.Value.bCallUpdateTowersInfo = true;

										//通知客户端更新装备CD
										StoneDefenceUtils::CallUpdateAllClient(GetWorld(), [&](ATowerDefencePlayerController* InPlayerController)
											{
												InPlayerController->UpdateInventory_Client(BuildingTowerPair.Key, true);
											});
									}
									else if (BuildingTowerPair.Value.bCallUpdateTowersInfo)
									{
										BuildingTowerPair.Value.bCallUpdateTowersInfo = false;
										//准备构建的塔
										BuildingTowerPair.Value.TowersPrepareBuildingNumber--;
										BuildingTowerPair.Value.TowersConstructionNumber++;

										//通知客户端更新装备的CD
										StoneDefenceUtils::CallUpdateAllClient(GetWorld(), [&](ATowerDefencePlayerController* InPlayerController)
											{
												InPlayerController->UpdateInventory_Client(BuildingTowerPair.Key, false);
											});

										if (BuildingTowerPair.Value.TowersPrepareBuildingNumber > 0)
										{
											BuildingTowerPair.Value.ResetCD();
										}
									}
								}
							}
						}
					}
				}
			});

	}
}

//打开优化
#if PLATFORM_WINDOWS
#pragma optimize("", on)
#endif