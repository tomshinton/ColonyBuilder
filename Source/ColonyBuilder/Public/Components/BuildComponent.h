// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "RTSBaseComp.h"

#include "Utils/DataTypes/BuildingDataTypes.h"

#include "BuildComponent.generated.h"

class UBuildingData;
class AGhost;

class ARTSPlayerController;
class APlayerPawn;

DECLARE_LOG_CATEGORY_EXTERN(BuildCompLogError, Error, All);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COLONYBUILDER_API UBuildComponent : public URTSBaseComp
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBuildComponent();

	void RotatePlacement();

	void StartBuildingFromClass(UBuildingData* BuildingData);
	UFUNCTION()
	void StartPlacement(bool IsNewPlacement = true);
	UFUNCTION()
	void EndPlacement();
	UFUNCTION()
	void CancelBuild();

	void BuildIntermediatePositions();
	void AlignAndOrientate();
	void ValidatePointTypesToUnique();
	
	TArray<FSubBuilding> BuildFFPoints();
	TArray<FSubBuilding> BuildLinearPoints();
	TArray<FSubBuilding> BuildGridPoints();

	FTimerHandle BuildIntermediatePosTimer;
	
	UBuildingData* BuildingData;
	TArray<FSubBuilding> SubBuildings;

private:

	FVector CurrMouseCoords;
	FVector CurrRoundedMouseCoords;
	FVector MouseLocationAtBuildingStart;
	void UpdateGhost();
	AGhost* SpawnedGhost;

	float RotationRate = 45.f;

	bool HasStartedBuilding;
	
};
