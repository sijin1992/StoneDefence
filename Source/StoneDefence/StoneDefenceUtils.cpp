#include "StoneDefenceUtils.h"
#include "Public/Character/Core/RuleOfTheCharacter.h"
#include "Interface/Character/RuleCharacter.h"
#include "Engine/StaticMesh.h"
#include "RawMesh/Public/RawMesh.h"
#include "Rendering/SkeletalMeshRenderData.h"
#include "Rendering/SkeletalMeshLODRenderData.h"
#include "Components/SkeletalMeshComponent.h"
#include "SkeletalRenderPublic.h"
//关闭优化optimize
#if PLATFORM_WINDOWS
#pragma optimize("",off)
#endif

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

float Expression::GetDamage(IRuleCharacter* Enemy, IRuleCharacter* Owner)
{
	if (Enemy && Owner)
	{
		return Enemy->GetCharacterData().PhysicalAttack / ((Owner->GetCharacterData().Armor / 100.0f) + 1);
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



UStaticMesh* MeshUtils::SkeletalMeshToStaticMesh(UWorld* InWorld, USkeletalMeshComponent* SkeletalMeshComponent)
{
	UStaticMesh* StaticMesh = nullptr;

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

	return StaticMesh;
}

//打开优化
#if PLATFORM_WINDOWS
#pragma optimize("",on)
#endif
