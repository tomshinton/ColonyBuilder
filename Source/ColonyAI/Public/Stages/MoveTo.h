// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "Plan/Stage.h"
#include "MoveTo.generated.h"

class USplineComponent;

//////////////////////////////////////////////////////////////////////////
// MoveTo - move this owner to said location by querying the navigationsys
//////////////////////////////////////////////////////////////////////////

UCLASS()
class COLONYAI_API UMoveTo : public UStage
{
	GENERATED_BODY()
	
public:

	UMoveTo();

	virtual void OnStageTick(const float DeltaTime) override;
	virtual void OnStart() override;

private:

	UPROPERTY()	
	USplineComponent* PathSpline;

	FNavAgentProperties NavProperties;
	FPathFindingQuery NavQuery;

	bool ShouldPath;

	float CurrentDistance;
	float MaxDistance;

	void GetRoute();
	void OnRouteFound(uint32 PathId, ENavigationQueryResult::Type ResultType, FNavPathSharedPtr NavPointer);
};
