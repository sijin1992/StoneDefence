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
}

void AStoneDefenceGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (ATowerDefenceGameState* InGameState = GetGameState<ATowerDefenceGameState>())
	{
		//这里是因为服务器有多个PlayerController,所以需要遍历多个PlayerController
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			//这里是因为是单机，所以直接转了，如果是网络游戏需要判断是否是要找的PlayerController
			if (APlayerController* MyPlayerController = It->Get())
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
		}

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
					if (InGameState->GetGameData().IsAlloSpawnMonster())
					{
						InGameState->GetGameData().ResetSpawnMonsterTime();

						int32 MonsterLevel = 1;//GetMonsterLevel(GetWorld());
						if (ARuleOfTheCharacter* MyMonster = SpawnMonster(0, MonsterLevel, FVector::ZeroVector, FRotator::ZeroRotator))
						{
							TArray<ASpawnPoint*> MonsterSpawnPoints;
							for (ASpawnPoint* TargetPoint : StoneDefenceUtils::GetAllActor<ASpawnPoint>(GetWorld()))
							{
								if (MyMonster->IsTeam() == TargetPoint->bTeam)
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
					DifficultyDetermination.Attack += Temp->GetCharacterData().PhysicalAttack;
					DifficultyDetermination.Defence += Temp->GetCharacterData().Armor;
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

	int32 ReturnLevel = TowerDD.Level;
	if (TowerDD.Attack > MonsterDD.Attack)
	{
		ReturnLevel++;
	}
	if (TowerDD.Defence > MonsterDD.Defence)
	{
		ReturnLevel++;
	}

	ReturnLevel += FMath::Abs(2 - FMath::Sqrt(TowerDD.Variance));//2是保护值,对方差开根

	return ReturnLevel;
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

			auto GetCharacterData = [&](int32 ID) ->FCharacterData*
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


			if (FCharacterData* CharacterData = GetCharacterData(CharacterID))
			{
				UClass* NewClass = CharacterData->CharacterBlueprintKey.LoadSynchronous();//LoadSynchronous读取
				if (GetWorld() && NewClass)
				{
					if (ARuleOfTheCharacter* RuleOfTheCharacter = GetWorld()->SpawnActor<ARuleOfTheCharacter>(NewClass, Location, Rotator))
					{
						CharacterData->UpdateHealth();

						if (CharacterLevel > 1)
						{
							//升极
							for (int32 i = 0; i < CharacterLevel; i++)
							{
								CharacterData->UpdateLevel();
							}
						}
						//RuleOfTheCharacter->GUID = FGuid::NewGuid();
						InGameState->AddCharacterData(RuleOfTheCharacter->GUID, *CharacterData);
						//AddCharacterData(RuleOfTheCharacter->GetUniqueID(), *CharacterData);
						OutCharacter = RuleOfTheCharacter;
					}
				}
			}
		}
	}
	return OutCharacter;
}
