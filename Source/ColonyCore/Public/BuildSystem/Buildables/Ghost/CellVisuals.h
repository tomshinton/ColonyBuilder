// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/InstancedStaticMeshComponent.h>
#include "CellVisuals.generated.h"

struct FSubBuilding;

//////////////////////////////////////////////////////////////////////////
// Visuals for a single grid cell - also displays useful diagnostic info like why a cell is invalid
//////////////////////////////////////////////////////////////////////////

UCLASS()
class COLONYCORE_API ACellVisuals : public AActor
{
	GENERATED_BODY()

public:	
	ACellVisuals();	

	UPROPERTY()
	USceneComponent* VisualRoot;

	UPROPERTY()
	UInstancedStaticMeshComponent* Cells;

	void Visualise(const TArray<FSubBuilding>& InPointsToRepresent);

private:

	UPROPERTY()
	UStaticMesh* CellMesh;

	UPROPERTY()
	UMaterialInterface* DecalMat;

	UPROPERTY()
	TArray<UDecalComponent*> SpawnedDecals;
};
