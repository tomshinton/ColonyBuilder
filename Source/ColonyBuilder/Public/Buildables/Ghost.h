// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/SplineComponent.h"

#include "DataTypes/BuildingDataTypes.h"

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

	UHierarchicalInstancedStaticMeshComponent* InstancedMeshes;
	UHierarchicalInstancedStaticMeshComponent* UniqueInstancedMeshes;

	USplineComponent* SplineComp;
	
public:	
	
	AGhost();

	void Tick(float DeltaTime);

	static const float ValidationCheckFreq;
	
	void SetValid(bool InValidState);
	void SetGhostMaterial(UMaterialInterface* NewMaterial);
	bool GetIsValid() { return IsValid; }
		
	void SetGhostInfo(UBuildingData* InBuildingData);
	UBuildingData* BuildingData;
	FTimerHandle CheckPlacementTimer;

	void CheckPlacement();

	void UpdateGhost(FVector NewLocation, TArray<FSubBuilding>& SubBuildings);

	//Build mock body actors like plantables/beehives, etc etc
	void UpdateGridGhost();
	//Build mock path/wall
	void UpdateLinearGhost();
	//Update standard ghost
	void UpdateFireAndForgetGhost();

	TArray<FSubBuilding> SubBuildings;
	
private:
	bool IsValid;

	TArray<USplineMeshComponent*> SpawnedMeshComponents;

};
