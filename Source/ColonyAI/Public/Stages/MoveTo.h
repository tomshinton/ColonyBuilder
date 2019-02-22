// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "Plan/Stage.h"

#include "MoveTo.generated.h"

class USplineComponent;

//////////////////////////////////////////////////////////////////////////
// MoveTo - move this owner to said location by querying the navigationsys
//////////////////////////////////////////////////////////////////////////

DECLARE_LOG_CATEGORY_EXTERN(MoveToLog, All, All)

UCLASS()
class COLONYAI_API UMoveTo : public UStage
{
	GENERATED_BODY()
	
public:

	UMoveTo();

	virtual void OnStageTick(const float DeltaTime) override;
	virtual void OnStart() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MoveToLocation)
	bool ShouldGarrisonOnEnd;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MoveToLocation)
	FVector MoveToLocation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MoveToLocation)
		FGuid TargetBuildingID;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage Flow")
	void SetMoveToLocation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage Flow")
	void SetTargetBuildingID();

protected:

	virtual void OnSetMoveToLocation();

private:

	UPROPERTY()	
	USplineComponent* PathSpline;

	FNavAgentProperties NavProperties;
	FPathFindingQuery NavQuery;

	bool ShouldPath;

	float CurrentDistance;
	float MaxDistance;

	float AdhereFrequency;

	void GetRoute();
	void OnRouteFound(uint32 PathId, ENavigationQueryResult::Type ResultType, FNavPathSharedPtr NavPointer);

	void AdherePoint(FVector& InPointToAdhere);
	void OnAdhereComplete();

	void ProjectLocationToNavMesh();

	bool CanFinishMove() const;
	void FinishMove();

private:
		
	void GarrisonPawn();
	void UngarrisonPawn();
	
	FTraceDelegate OnTraceCompleteDelegate;

	int32 ExpectedAdheredPoints;
	TArray<FVector> AdheredPoints;
};
