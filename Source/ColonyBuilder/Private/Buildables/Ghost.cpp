// ColonyBuilder Project, personal project by Tom Shinton

#include "Ghost.h"
#include "BuildingData.h"
#include "Kismet/KismetSystemLibrary.h"

#include "DrawDebugHelpers.h"
#include "ColonyBuilderGameModeBase.h"
#include "Engine/StaticMesh.h"

#include "Utils/DebugUtils.h"
#include "Utils/GridUtils.h"

const float AGhost::ValidationCheckFreq(0.05);

DEFINE_LOG_CATEGORY(GhostError);

// Sets default values
AGhost::AGhost()
{
	PrimaryActorTick.bCanEverTick = true;
	SetActorEnableCollision(false);

	GhostRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Ghost Root"));
	GhostRoot->SetMobility(EComponentMobility::Movable);
	RootComponent = GhostRoot;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->SetupAttachment(GhostRoot);

	MeshComp->SetCollisionProfileName(TEXT("OverlapAll"));

	InstancedMeshes = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("Instanced Meshes"));
	InstancedMeshes->SetupAttachment(GhostRoot);

	UniqueInstancedMeshes = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("Unique Instanced Meshes"));
	UniqueInstancedMeshes->SetupAttachment(GhostRoot);

	SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Comp"));
	SplineComp->SetupAttachment(GhostRoot);
}

void AGhost::Tick(float DeltaTime)
{
	CheckPlacement();
}

void AGhost::SetGhostInfo(UBuildingData* InBuildingData)
{
	FVector BoundsOrigin;

	BuildingData = InBuildingData;

	GetActorBounds(false, BoundsOrigin, CachedGhostBounds);
	MeshComp->SetStaticMesh(BuildingData->BuildingBaseMesh);
}

void AGhost::CheckPlacement()
{
	SetValid(true);

	FVector ActorBounds;
	FVector ActorCentre;

	for (FSubBuilding& SubBuilding : SubBuildings)
	{
		TArray<EInvalidReason> Reasons = UGridUtils::IsPointValid(this, SubBuilding, BuildingData);

		if (!SubBuilding.IsValidPoint)
		{
			if (Reasons.Num() == 1 && Reasons.Contains(EInvalidReason::LegalOverlap)){}
			else
			{			
				SetValid(false);
			}

			for (int32 i = 0; i <= Reasons.Num() -1; i++)
			{
				const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EInvalidReason"), true);
				FString ReasonString = EnumPtr->GetNameByValue((int64)Reasons[i]).ToString();

				GEngine->AddOnScreenDebugMessage(i, 1, FColor::White, ReasonString);
			}
		}
	}
}

void AGhost::SetValid(bool InValidState)
{
	if (InValidState)
	{
		SetGhostMaterial(BuildingData->ValidGhostMaterial);
		IsValid = true;
	}
	else
	{
		SetGhostMaterial(BuildingData->InvalidGhostMaterial);
		IsValid = false;
	}
}

void AGhost::SetGhostMaterial(UMaterialInterface* NewMaterial)
{
	//Base Meshes, like stand alones
	for (int8 MatID = 0; MatID <= MeshComp->GetMaterials().Num() - 1; MatID++)
	{
		MeshComp->SetMaterial(MatID, NewMaterial);
	}

	//Instances, for stuff like grids.
	if (InstancedMeshes->GetStaticMesh())
	{
		for (int8 MatID = 0; MatID <= InstancedMeshes->GetStaticMesh()->StaticMaterials.Num(); MatID++)
		{
			InstancedMeshes->SetMaterial(MatID, NewMaterial);
		}
	}

	//Instances, for stuff like wall posts.
	if (UniqueInstancedMeshes->GetStaticMesh())
	{
		for (int8 MatID = 0; MatID <= UniqueInstancedMeshes->GetStaticMesh()->StaticMaterials.Num(); MatID++)
		{
			UniqueInstancedMeshes->SetMaterial(MatID, NewMaterial);
		}
	}
}

void AGhost::UpdateGhost(FVector NewLocation, TArray<FSubBuilding>& InSubBuildings)
{

	if (NewLocation != GetActorLocation())
	{
		SetActorLocation(NewLocation);
	}

	SubBuildings = InSubBuildings;

	switch (BuildingData->ConstructionMethod)
	{
	case EConstructionMethod::FireAndForget:
		break;
	case EConstructionMethod::Grid:
		UpdateGridGhost();
		break;
	case EConstructionMethod::Linear:
		UpdateLinearGhost();
		break;
	}
}

void AGhost::UpdateGridGhost()
{
	InstancedMeshes->ClearInstances();

	if (BuildingData && BuildingData->BodyClass)
	{
		UStaticMesh* DefaultMesh = Cast<AGridBodyBase>(BuildingData->BodyClass->GetDefaultObject())->StaticMeshComp->GetStaticMesh();
		if (DefaultMesh)
		{
			InstancedMeshes->SetStaticMesh(DefaultMesh);
			FTransform NewInstanceTrans;

			for (FSubBuilding& SubBuilding : SubBuildings)
			{
				NewInstanceTrans.SetLocation(SubBuilding.Location);
				NewInstanceTrans.SetRotation(FRotator(0, 0, 0).Quaternion());
				NewInstanceTrans.SetScale3D(FVector(1, 1, 1));

				InstancedMeshes->AddInstanceWorldSpace(NewInstanceTrans);
			}
		}
	}
}

void AGhost::UpdateLinearGhost()
{
	InstancedMeshes->ClearInstances();
	UniqueInstancedMeshes->ClearInstances();

	SplineComp->ClearSplinePoints(true);

	UStaticMesh* GeneralMesh = *BuildingData->SubBuildingMeshes.Find(ESubBuildingType::SplineGeneral);
	UStaticMesh* UniqueMesh = *BuildingData->SubBuildingMeshes.Find(ESubBuildingType::SplineUnique);

	FVector GeneralMeshSize;

	if (!GeneralMesh)
	{
		UE_LOG(GhostError, Log, TEXT("No valid linear ghost mesh supplied"));
		return;
	}
	else
	{
		GeneralMeshSize = GeneralMesh->GetBoundingBox().GetExtent();
	}

	if (SubBuildings.Num() <= 0)
	{
		UE_LOG(GhostError, Log, TEXT("No spline points yet"));
		return;
	}

	for (int32 point = 0; point <= SubBuildings.Num() - 1; point++)
	{
		SplineComp->AddSplineWorldPoint(SubBuildings[point].Location);
		SplineComp->SetSplinePointType(point, ESplinePointType::Linear, true);
	}

	const int32 MeshesOverSpline = SplineComp->GetSplineLength() / GeneralMeshSize.X;

	InstancedMeshes->SetStaticMesh(GeneralMesh);
	UniqueInstancedMeshes->SetStaticMesh(UniqueMesh);

	for (int32 i = 0; i <= SubBuildings.Num() - 1; i++)
	{
		FTransform NewMeshTransform;

		NewMeshTransform.SetLocation(SubBuildings[i].Location);
		NewMeshTransform.SetRotation(FVector(SubBuildings[i].Direction.X, SubBuildings[i].Direction.Y, 0).Rotation().Quaternion());
		NewMeshTransform.SetScale3D(FVector(1, 1, 1));

		if (SubBuildings[i].SubBuildingType == ESubBuildingType::SplineUnique)
		{
			UniqueInstancedMeshes->AddInstanceWorldSpace(NewMeshTransform);
		}
		else
		{
			InstancedMeshes->AddInstanceWorldSpace(NewMeshTransform);
		}
	}
}
