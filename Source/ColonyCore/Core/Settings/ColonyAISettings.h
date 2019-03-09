#pragma once

#include "ColonyAISettings.generated.h"

class UStage;

//////////////////////////////////////////////////////////////////////////
// Base UObject for AI settings - accessed via GetUnmutableDefaults
//////////////////////////////////////////////////////////////////////////

UCLASS(config = Game, defaultconfig)
class COLONYCORE_API UColonyAISettings : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, config, Category = "Intervals", meta = (ClampMin = "0", UIMin = "0"))
	float PlanAdvanceInterval;

	/** Static stages are stages that are ran whenever the plan is ticked - they act as setup functions, e.g FindWork, FindHouse, etc etc*/
	/** They do not get popped off the stack of stages in a plan*/
	UPROPERTY(EditAnywhere, config, Category = "Recursive Stages")
	TArray<TSubclassOf<UStage>> RecusiveStages;

	UPROPERTY(EditAnywhere, config, Category = "Recursive Stages")
	float RecursivePlanFrequency;
};
