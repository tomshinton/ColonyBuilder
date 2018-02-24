// ColonyBuilder Project, personal project by Tom Shinton

#include "Ghost.h"
#include "BuildingData.h"
#include "Kismet/KismetSystemLibrary.h"

#include "DrawDebugHelpers.h"
#include "ColonyBuilderGameModeBase.h"

const float AGhost::ValidationCheckFreq(0.05);

// Sets default values
AGhost::AGhost()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	RootComponent = MeshComp;

	MeshComp->SetCollisionProfileName(TEXT("OverlapAll"));

}

void AGhost::OnConstruction(const FTransform& Transform)
{

}

void AGhost::SetGhostInfo(UBuildingData* InBuildingData)
{
	BuildingData = InBuildingData;
	MeshComp->SetStaticMesh(InBuildingData->BuildingBaseMesh);

	GetWorld()->GetTimerManager().SetTimer(CheckPlacementTimer, this, &AGhost::CheckPlacement, AGhost::ValidationCheckFreq, true);
}

void AGhost::CheckPlacement()
{
	GEngine->AddOnScreenDebugMessage(-1, AGhost::ValidationCheckFreq, FColor::White, "Checking ghost placement");

	//Check if we're overlapping with anything
	TArray<AActor*> OverlappingActors;
	GetObstructingActors(OverlappingActors);

	if (OverlappingActors.Num() > 0)
	{
		SetValid(false);
		return;
	}
	//Uneven terrain

	//Not on the correct surface type eg On a resource

	//Unsuitable ground type, such as the ground not being fertile for farms

	//HeightCheck, prevent this building from being built below the water table



	/*DEBUG*/
	FlushPersistentDebugLines(GetWorld());
	FlushDebugStrings(GetWorld());

	for (FIntermediateBuildingLocation Loc : IntermediateBuildingLocations)
	{
		DrawDebugSphere(GetWorld(), Loc.Location, AColonyBuilderGameModeBase::GridSize / 2, 8, FColor::Emerald, false, 0.5f);
		DrawDebugString(GetWorld(), Loc.Location, Loc.ToStringFromInts(), NULL, FColor::White, 0.5f, true);
	}

	SetValid(true);
}

void AGhost::SetValid(bool InValidState)
{
	if (InValidState)
	{
		for (int8 MatID = 0; MatID <= MeshComp->GetMaterials().Num() - 1; MatID++)
		{
			MeshComp->SetMaterial(MatID, BuildingData->ValidGhostMaterial);
			IsValid = true;
		}
	}
	else
	{
		for (int8 MatID = 0; MatID <= MeshComp->GetMaterials().Num() - 1; MatID++)
		{
			MeshComp->SetMaterial(MatID, BuildingData->InvalidGhostMaterial);
			IsValid = false;
		}
	}
}

void AGhost::GetObstructingActors(TArray<AActor*>& ObstructingActors)
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);

	//Allow for additional checks here, do we want to remove actors from this list?

	ObstructingActors = OverlappingActors;
}

void AGhost::UpdateGhost(FVector NewLocation, TArray<FIntermediateBuildingLocation> InIntermediateBuildingLocations)
{
	if (NewLocation != GetActorLocation())
	{
		SetActorLocation(NewLocation);
	}

	IntermediateBuildingLocations = InIntermediateBuildingLocations;
}

