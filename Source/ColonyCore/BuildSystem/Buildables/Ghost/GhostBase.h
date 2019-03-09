// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

class UBoxComponent;
class UHierarchicalInstancedStaticMeshComponent;
class USplineMeshComponent;
class USplineComponent;
class UBuildingData;

#include "Utils/DataTypes/BuildingDataTypes.h"
#include "Ghost.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(GhostLog, Log, All);

UCLASS()
class COLONYCORE_API AGhost : public AActor
{
	GENERATED_BODY()

	AGhost();

	USceneComponent* GhostRoot;

	UStaticMeshComponent* MeshComp;
	UBoxComponent* BoundsComp;

	UHierarchicalInstancedStaticMeshComponent* BodyMeshes;
	UHierarchicalInstancedStaticMeshComponent* UniqueMeshes;
	UHierarchicalInstancedStaticMeshComponent* LinkMeshes;
	UHierarchicalInstancedStaticMeshComponent* TerminatorMeshes;

	USplineComponent* SplineComp;
	
public:	
		
	void SetValid(bool InValidState);
	void SetGhostMaterial(UMaterialInterface* NewMaterial);
	bool GetIsValid() { return IsValid; }

	void SetBaseGhostVisibility(bool NewHidden) { MeshComp->SetVisibility(NewHidden); }

	FVector GetCachedGhostBounds() { return CachedGhostBounds; }
	FRotator GetMeshCompRotation() const { return MeshComp->GetComponentRotation(); }
		
	void SetGhostInfo(UBuildingData* InBuildingData);
	UBuildingData* BuildingData;
	FTimerHandle CheckPlacementTimer;

	void CheckPlacement();

	void UpdateGhost(const FVector& NewLocation, const TArray<FSubBuilding>& SubBuildings);

	//Build mock body actors like plantables/beehives, etc etc
	void UpdateGridGhost();
	//Build mock path/wall
	void UpdateLinearGhost();

	TArray<FSubBuilding> SubBuildings;
	
private:
	bool IsValid;

	FVector CachedGhostBounds;

	TArray<USplineMeshComponent*> SpawnedMeshComponents;

	TArray<UHierarchicalInstancedStaticMeshComponent*> InstancedMeshes;

};
