// ColonyBuilder Project, personal project by Tom Shinton

#pragma once
#include <WorldCollision.h>
#include "BuildingDataTypes.h"

class AGhost;
struct FSubBuilding;
class ACellVisuals;

#define ValidVisuals 1

DECLARE_LOG_CATEGORY_EXTERN(ValidationRunnerLog, All, All)

class FValidationRunner
{

public:
	FValidationRunner(AGhost* InValidatingGhost);
	~FValidationRunner();

	void Validate(UWorld& InWorld, const TSubclassOf<AGhost>& InGhostClass, const TArray<FSubBuilding>& InTestPoints, const UBuildingData& InBuildingData);

	int32 TracesComplete;

	DECLARE_DELEGATE_OneParam(FOnFootprintMatchComplete, const TArray<FSubBuilding>&)
	FOnFootprintMatchComplete OnFootprintMatchComplete;

	DECLARE_DELEGATE_OneParam(FOnGroundAlignmentComplete, const TArray<FSubBuilding>&)
	FOnGroundAlignmentComplete OnGroundAlignComplete;

	DECLARE_DELEGATE_TwoParams(FOnValidateComplete, const bool, const TArray<FSubBuilding>&)
	FOnValidateComplete OnValidationComplete;

private:

	void FootprintMatch(UWorld& InWorld, const TArray<FSubBuilding>& InTestPoints);

	void AlignToGround(UWorld& InWorld, const TArray<FSubBuilding>& InTestPoints);

	void ApplyPointRules(UWorld& InWorld, const TArray<FSubBuilding>& InTestPoints, const UBuildingData& InBuildingData);

	void IsLegalOverlap(FSubBuilding& OutTestPoint, const FTraceDatum& InTraceDataconst, const UBuildingData& InBuildingData);

	void CreateCellVisuals(UWorld& InWorld, const TArray<FSubBuilding>& InPointToVisualise);

	UPROPERTY()
	AGhost* ValidatingGhost;

	UPROPERTY()
	ACellVisuals* CellVisuals;

	TArray<FSubBuilding> TestedPoints;

	FOverlapDelegate OnGhostOverlapCheckDone;
	FTraceDelegate OnTraceForGroundDelegate;
};