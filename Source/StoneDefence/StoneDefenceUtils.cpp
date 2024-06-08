#include "StoneDefenceUtils.h"
#include "Public/Character/Core/RuleOfTheCharacter.h"
#include "Interface/Character/RuleCharacter.h"
#include "Engine/StaticMesh.h"
#include "RawMesh/Public/RawMesh.h"
#include "Rendering/SkeletalMeshRenderData.h"
#include "Rendering/SkeletalMeshLODRenderData.h"
#include "Components/SkeletalMeshComponent.h"
#include "SkeletalRenderPublic.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleEmitter.h"
#include "Particles/ParticleLODLevel.h"
#include "Particles/TypeData/ParticleModuleTypeDataMesh.h"
#include "Core/GameCore/TowerDefenceGameState.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#include "Components/ArrowComponent.h"
#include "Bullet/RuleOfTheBullet.h"
#include "Core/GameCore/TowerDefencePlayerController.h"

//关闭优化optimize
#if PLATFORM_WINDOWS
#pragma optimize("",off)
#endif

void StoneDefenceUtils::FindRangeTargetRecently(ARuleOfTheCharacter* InOwner, float Range, TArray<ARuleOfTheCharacter*>& Targets)
{
	if (InOwner && Range > 0.0f)
	{
		TArray<ARuleOfTheCharacter*> NewTargets;
		StoneDefenceUtils::GetAllActor(InOwner->GetWorld(), NewTargets);
		for (ARuleOfTheCharacter* Temp : NewTargets)
		{
			if (InOwner->GetTeamType() != Temp->GetTeamType())
			{
				if ((InOwner->GetActorLocation() - Temp->GetActorLocation()).Size() <= Range)
				{
					Targets.Add(Temp);
				}
			}
		}
	}
}

ARuleOfTheCharacter* StoneDefenceUtils::FindTargetRecently(const TArray<ARuleOfTheCharacter*> &InCharacters, const FVector& CompareLocation)
{
	if (InCharacters.Num())
	{
		float TargetDistance = 99999999.0f;
		int32 TargetIndex = INDEX_NONE;//-1
		for (int32 i = 0; i < InCharacters.Num(); i++)
		{
			if (ARuleOfTheCharacter* TargetCharacter = InCharacters[i])
			{
				FVector TargetLocation = TargetCharacter->GetActorLocation();
				FVector TmpVector = TargetLocation - CompareLocation;
				float TargetAndSelfDistance = TmpVector.Size();

				if (TargetAndSelfDistance < TargetDistance && TargetCharacter->IsActive())
				{
					TargetIndex = i;
					TargetDistance = TargetAndSelfDistance;
				}
			}
		}

		if (TargetIndex != INDEX_NONE)
		{
			return InCharacters[TargetIndex];
		}
	}

	return NULL;
}

void StoneDefenceUtils::FindCharacterToExecution(UWorld* InWorld, const FGuid CharacterFGuid, TFunction<void(ARuleOfTheCharacter* InCharacter)> Code)
{
	TArray<ARuleOfTheCharacter*> Characters;
	GetAllActor(InWorld, Characters);
	for (ARuleOfTheCharacter* Temp : Characters)
	{
		if (Temp->GUID == CharacterFGuid)
		{
			Code(Temp);
			break;
		}
	}
}

void StoneDefenceUtils::CallUpdateAllClient(UWorld* InWorld, TFunction<void(ATowerDefencePlayerController* MyPlayerController)> InImplement)
{
	if (InWorld)
	{
		//这里是因为服务器有多个PlayerController,所以需要遍历多个PlayerController
		for (FConstPlayerControllerIterator It = InWorld->GetPlayerControllerIterator(); It; ++It)
		{
			//这里是因为是单机，所以直接转了，如果是网络游戏需要判断是否是要找的PlayerController
			if (ATowerDefencePlayerController* MyPlayerController = Cast<ATowerDefencePlayerController>(It->Get()))
			{
				InImplement(MyPlayerController);
			}
		}
	}
}

ARuleOfTheBullet* StoneDefenceUtils::SpawnBullet(UWorld* InWorld, FGuid CharacterFGuid, UClass* InClass)
{
	TArray<ARuleOfTheCharacter*> InCharacterList;
	GetAllActor(InWorld, InCharacterList);

	for (auto& Temp : InCharacterList)
	{
		if (Temp->GUID == CharacterFGuid)
		{
			return SpawnBullet(InWorld, Temp, InClass, Temp->GetOpenFirePoint()->GetComponentLocation(), Temp->GetOpenFirePoint()->GetComponentRotation());
		}
	}
	return nullptr;
}

ARuleOfTheBullet* StoneDefenceUtils::SpawnBullet(UWorld* InWorld, ARuleOfTheCharacter* Instigator, const int32 SkillID, const FVector& InLocation, const FRotator& InRotation)
{
	ARuleOfTheBullet* NewBullet = nullptr;
	if (InWorld)
	{
		if (ATowerDefenceGameState* InGamestate = InWorld->GetGameState<ATowerDefenceGameState>())
		{
			if (const FSkillData* InSkillData = InGamestate->GetSkillData(SkillID))
			{
				if (ARuleOfTheBullet* Bullet = SpawnBullet(InWorld, Instigator, InSkillData->BulletClass, InLocation, InRotation))
				{
					NewBullet = Bullet;
				}
			}
		}
	}

	return NewBullet;
}

ARuleOfTheBullet* StoneDefenceUtils::SpawnBullet(UWorld* InWorld, APawn* NewPawn, UClass* InClass, const FVector& InLocation, const FRotator& InRotation)
{
	if (InWorld && NewPawn && InClass)
	{
		//构造Transform
		FTransform Transform;
		Transform.SetLocation(InLocation);
		Transform.SetRotation(InRotation.Quaternion());//将Rotation转化成四元数类型:TQuat
		//构造ActorSpawnParameters
		FActorSpawnParameters ActorSpawnParameters;
		ActorSpawnParameters.Instigator = NewPawn;//设置施法者

		if (ARuleOfTheBullet* Bullet = InWorld->SpawnActor<ARuleOfTheBullet>(InClass, Transform, ActorSpawnParameters))
		{
			return Bullet;
		}
	}
	return nullptr;
}

AStaticMeshActor* StoneDefenceUtils::SpawnTowersDoll(UWorld* InWorld, int32 ID)
{
	AStaticMeshActor* OutActor = nullptr;

	if (InWorld)
	{
		if (ATowerDefenceGameState* InGameState = InWorld->GetGameState<ATowerDefenceGameState>())
		{
			const TArray<FCharacterData*> InDatas = InGameState->GetTowerDataFromTable();
			//遍历塔的数据列表
			for (const auto& Temp : InDatas)
			{
				if (Temp->ID == ID)
				{
					//生成一个炮塔实例
					UClass* NewClass = Temp->CharacterBlueprintKey.LoadSynchronous();
					if (InWorld && NewClass)
					{
						if (ARuleOfTheCharacter* RuleOfTheCharacter = InWorld->SpawnActor<ARuleOfTheCharacter>(NewClass, FVector::ZeroVector, FRotator::ZeroRotator))
						{
							//生成替代模型
							if (AStaticMeshActor* MeshActor = InWorld->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator))
							{
								FTransform InTransform;
								//获取StaticMesh
								if (UStaticMesh* InMesh = RuleOfTheCharacter->GetDollMesh(InTransform))
								{
									MeshActor->GetStaticMeshComponent()->SetRelativeTransform(InTransform);
									MeshActor->SetMobility(EComponentMobility::Movable);//设置为可移动的组件
									MeshActor->GetStaticMeshComponent()->SetStaticMesh(InMesh);//将获取的StaticMesh设置给替代模型
									MeshActor->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);//设置为可移动的组件
									OutActor = MeshActor;
									RuleOfTheCharacter->Destroy();
								}
								else
								{
									MeshActor->Destroy();
									RuleOfTheCharacter->Destroy();
								}
							}
							else
							{
								RuleOfTheCharacter->Destroy();
							}
						}
					}
					break;
				}
			}
		}
	}

	return OutActor;
}

float Expression::GetDamage(IRuleCharacter* Enemy, IRuleCharacter* Owner)
{
	if (Enemy && Owner)
	{
		return Enemy->GetCharacterData().GetAttack() / ((Owner->GetCharacterData().GetArmor() / 100.0f) + 1);
	}
	return 0.0f;
}

struct FMeshTracker
{
	FMeshTracker()
		:bValidColors(false)
	{
		FMemory::Memset(bValidTexCoords, 0);//内存初始化
	}

	bool bValidTexCoords[MAX_MESH_TEXTURE_COORDS];//有效的UV,MAX_MESH_TEXTURE_COORDS = 8是个宏
	bool bValidColors;//有效的颜色
};

/// <summary>
/// 将骨骼网格体转换成原始网格数据
/// </summary>
/// <param name="InComponent"></param>
/// <param name="InOverallMaxLODs"></param>
/// <param name="InComponentToWorld"></param>
/// <param name="OutRawMeshTracker"></param>
/// <param name="InRawMesh"></param>
void SkeletalMeshToRawMeshes(
	USkeletalMeshComponent* InComponent,
	int32 InOverallMaxLODs,
	const FMatrix& InComponentToWorld,
	FMeshTracker& OutRawMeshTracker,
	FRawMesh& InRawMesh)
{
	//拿到LOD
	FSkeletalMeshLODInfo& SrcLODInfo = *(InComponent->SkeletalMesh->GetLODInfo(InOverallMaxLODs));
	//通过LOD拿到LOD对应的点数
	TArray<FFinalSkinVertex> FinalVertices;
	InComponent->GetCPUSkinnedVertices(FinalVertices, InOverallMaxLODs);
	//拿到渲染的数据
	FSkeletalMeshRenderData& SkeletalMeshRenderData = InComponent->MeshObject->GetSkeletalMeshRenderData();
	//拿到LOD所对应的渲染数据
	FSkeletalMeshLODRenderData& LODData = SkeletalMeshRenderData.LODRenderData[InOverallMaxLODs];

	//拷贝LOD对应的点数到原始网格数据里
	for (int32 VertIndex = 0; VertIndex < FinalVertices.Num(); ++VertIndex)
	{
		//拿到了顶点的位置,TransformPosition是用矩阵把局部坐标转换成世界坐标
		FVector3f v3fPos = FinalVertices[VertIndex].Position;
		FVector vector(v3fPos);
		FVector4 v4 = InComponentToWorld.TransformPosition(vector);
		FVector3f v3f(v4.X, v4.Y, v4.Z);
		InRawMesh.VertexPositions.Add(v3f);
	}
	//获取最大的UV数
	const uint32 NumTexCoords = FMath::Min(LODData.StaticVertexBuffers.StaticMeshVertexBuffer.GetNumTexCoords(), (uint32)MAX_MESH_TEXTURE_COORDS);
	//go编辑器开发继承课程
	const int32 NumSections = LODData.RenderSections.Num();
	//从LOD里面的索引容器里拿到索引的缓冲区
	FRawStaticIndexBuffer16or32Interface& IndexBuffer = *LODData.MultiSizeIndexContainer.GetIndexBuffer();
	
	//遍历Section
	for (int32 SectionIndex = 0; SectionIndex < NumSections; SectionIndex++)
	{
		//获取Section所对应的SkelMeshRenderSection
		const FSkelMeshRenderSection& SkelMeshSection = LODData.RenderSections[SectionIndex];
		if (InComponent->IsMaterialSectionShown(SkelMeshSection.MaterialIndex, InOverallMaxLODs))//获取LOD对应的MaterialIndex,然后看这个材质是不是被隐藏了
		{
			//构建三角锥信息,三角锥数量=三角形数*3
			const int32 NumWedges = SkelMeshSection.NumTriangles * 3;
			//遍历三角锥
			for (int32 WedgeIndex = 0; WedgeIndex < NumWedges; WedgeIndex++)
			{
				//获取顶点所对应的三角锥, BaseIndex是SkelMeshSection的偏移 然后加上三角锥Index
				const int32 VertexIndexForWedge = IndexBuffer.Get(SkelMeshSection.BaseIndex + WedgeIndex);
				//将其添加到原始网格数据里的三角锥索引里
				InRawMesh.WedgeIndices.Add(VertexIndexForWedge);

				//拿到蒙皮顶点,FinalVertices里有很多数据
				const FFinalSkinVertex& SkinnedVertex = FinalVertices[VertexIndexForWedge];
				//拿到切线的X、Y、Z轴,ToFVector舍弃了齐次坐标,转换成FVector,TransformVector是将局部空间转换成世界空间
				const FVector TangentX = InComponentToWorld.TransformVector(SkinnedVertex.TangentX.ToFVector());
				const FVector TangentZ = InComponentToWorld.TransformVector(SkinnedVertex.TangentZ.ToFVector());
				const FVector4 UnpackedTangentX = SkinnedVertex.TangentX.ToFVector4();
				//根据矩阵的乘法，切线Y垂直与切线Z和切线X，GetSafeNormal是单位化变成方向, UnpackedTangentZ.W是齐次坐标,然后根据下列公式求得
				// x = x/w;
				// y = y/w;
				const FVector TangentY = (TangentZ ^ TangentX).GetSafeNormal() * UnpackedTangentX.W;

				//将切线坐标都添加进RawMesh里
				InRawMesh.WedgeTangentX.Add(FVector3f(TangentX));
				InRawMesh.WedgeTangentY.Add(FVector3f(TangentY));
				InRawMesh.WedgeTangentZ.Add(FVector3f(TangentZ));

				//遍历UV，把SkelMesh里的UV转到StaticMesh里
				for (uint32 TexCoordIndex = 0; TexCoordIndex < MAX_MESH_TEXTURE_COORDS; TexCoordIndex++)
				{
					if (TexCoordIndex >= NumTexCoords)
					{
						InRawMesh.WedgeTexCoords[TexCoordIndex].AddDefaulted();
					}
					else
					{
						InRawMesh.WedgeTexCoords[TexCoordIndex].Add(LODData.StaticVertexBuffers.StaticMeshVertexBuffer.GetVertexUV(VertexIndexForWedge, TexCoordIndex));
						OutRawMeshTracker.bValidTexCoords[TexCoordIndex] = true;
					}
				}
				//判断当前LOD所对应的顶点着色是否已经初始化了
				if (LODData.StaticVertexBuffers.ColorVertexBuffer.IsInitialized())
				{
					InRawMesh.WedgeColors.Add(LODData.StaticVertexBuffers.ColorVertexBuffer.VertexColor(VertexIndexForWedge));
					OutRawMeshTracker.bValidColors = true;//记录是否有效的颜色为true
				}
				else
				{
					//没有初始化默认填充白色
					InRawMesh.WedgeColors.Add(FColor::White);
				}
			}

			//处理材质
			int32 MaterialIndex = SkelMeshSection.MaterialIndex;
			//当前SectionIndex是否是有效值
			if (SrcLODInfo.LODMaterialMap.IsValidIndex(SectionIndex) && SrcLODInfo.LODMaterialMap[SectionIndex] != INDEX_NONE)
			{
				//裁剪,LODMaterialMap里放着所有的材质索引,限制在0~材质的最大数
				MaterialIndex = FMath::Clamp<int32>(SrcLODInfo.LODMaterialMap[SectionIndex], 0, InComponent->SkeletalMesh->Materials.Num());
			}

			//拷贝,遍历SkelMeshSection里的三角形
			for (uint32 TriIndex = 0; TriIndex < SkelMeshSection.NumTriangles; TriIndex++)
			{
				//添加到RawMesh里
				InRawMesh.FaceMaterialIndices.Add(MaterialIndex);
				InRawMesh.FaceSmoothingMasks.Add(0);
			}
		}
	}
}

/// <summary>
/// 验证骨骼网格体是否有效
/// </summary>
/// <param name="InComponent"></param>
/// <returns></returns>
bool IsValidSkeletalMeshComponent(USkeletalMeshComponent* InComponent)
{
	return InComponent && InComponent->MeshObject && InComponent->IsVisible();
}


/// <summary>
/// 粒子特效组件转成静态模型
/// </summary>
/// <param name="NewParticleSystemComponent"></param>
/// <returns></returns>
UStaticMesh* MeshUtils::ParticleSystemToStaticMesh(UParticleSystemComponent* NewParticleSystemComponent)
{
	UStaticMesh* NewStaticMesh = nullptr;
	//如果粒子特效的Template有效且发射器数量大于0
	if (NewParticleSystemComponent->Template && NewParticleSystemComponent->Template->Emitters.Num() > 0)
	{
		//遍历发射器
		for (const UParticleEmitter* TempEmitter : NewParticleSystemComponent->Template->Emitters)
		{
			if (TempEmitter->LODLevels[0]->bEnabled)//细节层次是否开启
			{
				//获取粒子特效的数据类型，看看是否可以转成模型
				if (UParticleModuleTypeDataMesh* MyParticleDataMesh = Cast<UParticleModuleTypeDataMesh>(TempEmitter->LODLevels[0]->TypeDataModule))
				{
					if (MyParticleDataMesh->Mesh)
					{
						NewStaticMesh = MyParticleDataMesh->Mesh;
						break;
					}
				}
			}
		}
	}
	return NewStaticMesh;
}

/// <summary>
///骨骼模型转成静态模型 
/// </summary>
/// <param name="SkeletalMeshComponent"></param>
/// <returns></returns>
UStaticMesh* MeshUtils::SkeletalMeshToStaticMesh(USkeletalMeshComponent* SkeletalMeshComponent)
{
	UStaticMesh* StaticMesh = nullptr;
	if (UWorld* InWorld = SkeletalMeshComponent->GetWorld())
	{

		FRawMesh InRawMesh;//Mesh原始的数据，包括点的位置、颜色、坐标
		FMeshTracker MeshTracker;
		int32 OverallMaxLODs = 0;	//拿到网格最大的LOD

		const FTransform& InRootTransform = FTransform::Identity;//转换成单位矩阵
		FMatrix WorldToRoot = InRootTransform.ToMatrixWithScale().Inverse();//把Scale转换成矩阵，然后求这个矩阵的逆,也是单位矩阵
		//求出当前组件相对于世界的矩阵：用组件转过来的矩阵乘以WorldToRoot这个单位矩阵
		FMatrix ComponentToWorld = SkeletalMeshComponent->GetComponentTransform().ToMatrixWithScale() * WorldToRoot;

		if (IsValidSkeletalMeshComponent(SkeletalMeshComponent))
		{
			//*将骨骼网格体转换成原始网格数据*核心方法
			SkeletalMeshToRawMeshes(SkeletalMeshComponent, OverallMaxLODs, ComponentToWorld, MeshTracker, InRawMesh);
		}

		//声明一个最大的纹理坐标
		uint32 MaxInUseTextureCoordinate = 0;
		if (!MeshTracker.bValidColors)
		{
			//如果无效颜色，直接清空
			InRawMesh.WedgeColors.Empty();
		}
		//遍历当前的纹理坐标
		for (uint32 TexCoordIndex = 0; TexCoordIndex < MAX_MESH_TEXTURE_COORDS; TexCoordIndex++)
		{
			//当前TexCoord是否有效
			if (!MeshTracker.bValidTexCoords[TexCoordIndex])
			{
				//将三角锥直接清除
				InRawMesh.WedgeTexCoords[TexCoordIndex].Empty();
			}
			else
			{
				MaxInUseTextureCoordinate = FMath::Max(MaxInUseTextureCoordinate, TexCoordIndex);
			}
		}
		//判断RawMesh是否有意义
		if (InRawMesh.IsValidOrFixable())
		{
			FString MeshName = FGuid::NewGuid().ToString();//起个名字，防止重名
			StaticMesh = NewObject<UStaticMesh>(InWorld, *MeshName, RF_Transient);//在InWorld里生成对象，当InWorld销毁时，StaticMesh也会被销毁。RF_Transient标签表示当前对象不会被保存
			StaticMesh->InitResources();//初始化资源

			StaticMesh->SetLightingGuid(FGuid::NewGuid());
			//生成灯光贴图
			const uint32 LightMapIndex = FMath::Min(MaxInUseTextureCoordinate + 1, (uint32)8 - 1);//+1是灯光贴图材质，0是物体本身贴图材质

			//部署可以渲染为静态模型的模型元
			FStaticMeshSourceModel& SrcModel = StaticMesh->AddSourceModel();
			SrcModel.BuildSettings.bRecomputeNormals = false;//因为法线不需要重新计算了，所以忽略掉
			SrcModel.BuildSettings.bRecomputeTangents = false;//因为切线不需要重新计算了，所以忽略掉
			SrcModel.BuildSettings.bRemoveDegenerates = true;//移除无法生成的三角形
			SrcModel.BuildSettings.bUseHighPrecisionTangentBasis = false;//看切线是不是以16位对8位的精度来储存,置为false
			SrcModel.BuildSettings.bUseFullPrecisionUVs = false;//是不是要以全浮点精度来储存UV，置为false
			SrcModel.BuildSettings.bGenerateLightmapUVs = true;//是不是要生成灯光贴图
			SrcModel.BuildSettings.SrcLightmapIndex = 0;//灯光贴图默认给0
			SrcModel.BuildSettings.DstLightmapIndex = LightMapIndex;//目标光照贴图索引
			SrcModel.SaveRawMesh(InRawMesh);//将RawMesh添加到模型结构里

			for (const UMaterialInterface* Material : SkeletalMeshComponent->GetMaterials())
			{
				StaticMesh->GetStaticMaterials().Add(FStaticMaterial(const_cast<UMaterialInterface*>(Material)));
			}

			StaticMesh->ImportVersion = EImportStaticMeshVersion::LastVersion;//将导入的版本设置为上一次导入的版本
			StaticMesh->LightMapCoordinateIndex = LightMapIndex;//设置光照贴图索引

			//设置材质索引
			TArray<int32> UniqueMaterialIndices;
			for (int32 MaterialIndex : InRawMesh.FaceMaterialIndices)
			{
				UniqueMaterialIndices.AddUnique(MaterialIndex);
			}
			for (int32 i = 0; i < UniqueMaterialIndices.Num(); i++)
			{
				StaticMesh->GetSectionInfoMap().Set(0, i, FMeshSectionInfo(UniqueMaterialIndices[i]));
			}
			//拷贝
			StaticMesh->GetOriginalSectionInfoMap().CopyFrom(StaticMesh->GetSectionInfoMap());

			StaticMesh->Build(false);//Build给false，不希望弹出对话框
		}
	}

	return StaticMesh;
}

//打开优化
#if PLATFORM_WINDOWS
#pragma optimize("",on)
#endif
