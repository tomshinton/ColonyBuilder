// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/SplineComponent.h"

#include "Utils/DataTypes/BuildingDataTypes.h"

#include "Ghost.generated.h"

class UBuildingData;

DECLARE_LOG_CATEGORY_EXTERN(GhostError, Error, All);

UCLASS()
class COLONYBUILDER_API AGhost : public AActor
{
	GENERATED_BODY()

	USceneComponent* GhostRoot;

	UStaticMeshComponent* MeshComp;
	UBoxComponent* BoundsComp;

	UHierarchicalInstancedStaticMeshComponent* BodyMeshes;
	UHierarchicalInstancedStaticMeshComponent* UniqueMeshes;
	UHierarchicalInstancedStaticMeshComponent* LinkMeshes;
	UHierarchicalInstancedStaticMeshComponent* TerminatorMeshes;

	USplineComponent* SplineComp;
	
public:	
	
	AGhost();

	void Tick(float DeltaTime);

	static const float ValidationCheckFreq;
	
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

	void UpdateGhost(FVector NewLocation, TArray<FSubBuilding>& SubBuildings);

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
