// ColonyBuilder Project, personal project by Tom Shinton

#include "Ghost.h"
#include "BuildingData.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Core/GameMode/ColonyBuilderGameModeBase.h"

#include "DrawDebugHelpers.h"

#include "Utils/Libraries/DebugUtils.h"
#include "Utils/Libraries/GridUtils.h"

#include <Components/HierarchicalInstancedStaticMeshComponent.h>
#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>
#include "GridBodyBase.h"

DEFINE_LOG_CATEGORY(GhostLog);

// Sets default values
AGhost::AGhost()
	: GhostRoot(CreateDefaultSubobject<USceneComponent>(TEXT("Ghost Root")))
	, MeshComp(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp")))
	, BodyMeshes(CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("Body Meshes")))
	, UniqueMeshes(CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("Unique Meshes")))
	, LinkMeshes(CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("Link Meshes")))
	, TerminatorMeshes(CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("Terminator Meshes")))
{
	PrimaryActorTick.bCanEverTick = false;
	SetActorEnableCollision(false);

	GhostRoot->SetMobility(EComponentMobility::Movable);
	RootComponent = GhostRoot;

	MeshComp->SetupAttachment(GhostRoot);
	MeshComp->SetCollisionProfileName(TEXT("OverlapAll"));

	BodyMeshes->SetupAttachment(GhostRoot);
	UniqueMeshes->SetupAttachment(GhostRoot);
	LinkMeshes->SetupAttachment(GhostRoot);
	TerminatorMeshes->SetupAttachment(GhostRoot);

	InstancedMeshes.Add(BodyMeshes);
	InstancedMeshes.Add(UniqueMeshes);
	InstancedMeshes.Add(LinkMeshes);
	InstancedMeshes.Add(TerminatorMeshes);
}

void AGhost::SetGhostInfo(UBuildingData* InBuildingData)
{
	BuildingData = InBuildingData;
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

	for (UHierarchicalInstancedStaticMeshComponent* InstancedMeshComp : InstancedMeshes)
	{
		if (InstancedMeshComp->GetStaticMesh())
		{
			for (int8 MatID = 0; MatID <= InstancedMeshComp->GetStaticMesh()->StaticMaterials.Num(); MatID++)
			{
				InstancedMeshComp->SetMaterial(MatID, NewMaterial);
			}
		}
	}
}

void AGhost::UpdateGhost(const FVector& NewLocation, const TArray<FSubBuilding>& InSubBuildings)
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
	BodyMeshes->ClearInstances();

	if (BuildingData && BuildingData->BodyClass)
	{
		if (UStaticMesh* DefaultMesh = Cast<AGridBodyBase>(BuildingData->BodyClass->GetDefaultObject())->StaticMeshComp->GetStaticMesh())
		{
			BodyMeshes->SetStaticMesh(DefaultMesh);
			FTransform NewInstanceTrans;

			for (FSubBuilding& SubBuilding : SubBuildings)
			{
				NewInstanceTrans.SetLocation(SubBuilding.Location);
				NewInstanceTrans.SetRotation(FRotator(0, 0, 0).Quaternion());
				NewInstanceTrans.SetScale3D(FVector(1, 1, 1));

				BodyMeshes->AddInstanceWorldSpace(NewInstanceTrans);
			}
		}
	}
}

void AGhost::UpdateLinearGhost()
{
	BodyMeshes->ClearInstances();
	UniqueMeshes->ClearInstances();
	LinkMeshes->ClearInstances();
	TerminatorMeshes->ClearInstances();
	
	UStaticMesh* BodyMesh = *BuildingData->SubBuildingMeshes.Find(ESubBuildingType::LinearBody);
	UStaticMesh* UniqueMesh = *BuildingData->SubBuildingMeshes.Find(ESubBuildingType::LinearUnique);
	UStaticMesh* LinkMesh = *BuildingData->SubBuildingMeshes.Find(ESubBuildingType::LinearLink);
	UStaticMesh* TerminatorMesh = *BuildingData->SubBuildingMeshes.Find(ESubBuildingType::LinearTerminator);

	if (SubBuildings.Num() <= 0)
	{
		return;
	}

	BodyMeshes->SetStaticMesh(BodyMesh);
	UniqueMeshes->SetStaticMesh(UniqueMesh);
	LinkMeshes->SetStaticMesh(LinkMesh);
	TerminatorMeshes->SetStaticMesh(TerminatorMesh);

	for (int32 i = 0; i <= SubBuildings.Num() - 1; i++)
	{
		FTransform NewMeshTransform;

		NewMeshTransform.SetLocation(SubBuildings[i].Location);
		NewMeshTransform.SetRotation(FVector(SubBuildings[i].Direction.X, SubBuildings[i].Direction.Y, 0).Rotation().Quaternion());
		NewMeshTransform.SetScale3D(FVector(1, 1, 1));

		switch(SubBuildings[i].SubBuildingType)
		{
		case ESubBuildingType::LinearBody:
			BodyMeshes->AddInstanceWorldSpace(NewMeshTransform);
			break;
		case ESubBuildingType::LinearLink:
			LinkMeshes->AddInstanceWorldSpace(NewMeshTransform);
			break;
		case ESubBuildingType::LinearTerminator:
			TerminatorMeshes->AddInstanceWorldSpace(NewMeshTransform);
			break;
		case ESubBuildingType::LinearUnique:
			UniqueMeshes->AddInstanceWorldSpace(NewMeshTransform);
			break;
		}
	}
}
