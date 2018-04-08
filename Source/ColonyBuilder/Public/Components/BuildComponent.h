// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "RTSBaseComp.h"

#include "DataTypes/BuildingDataTypes.h"

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

	void BeginPlay();

	//Setters//
	virtual void SetEnabled(bool InEsnabled) override;

	UFUNCTION()
		void UpdateMouseCoords(FVector InCurrMouseCoords, FVector InRoundedMouseCoords);
	void RotatePlacement();

	void StartBuildingFromClass(UBuildingData* BuildingData);
	void StartPlacement(bool IsNewPlacement = true);
	void EndPlacement();
	void CancelBuild();

	void BuildIntermediatePositions();
	void AlignAndOrientate();
	void ValidatePointTypesToUnique();
	
	TArray<FSubBuilding> BuildLinearPoints();
	TArray<FSubBuilding> BuildGridPoints();
	bool PointIsInCorner(int32 PointX, int32 PointY, int32 MaxX, int32 MaxY);

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

	ARTSPlayerController* ControllerRef;
	
};
