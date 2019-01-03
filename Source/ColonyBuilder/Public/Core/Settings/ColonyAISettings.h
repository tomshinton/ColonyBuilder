#pragma once

#include "ColonyAISettings.generated.h"

class UStage;

//////////////////////////////////////////////////////////////////////////
// Base UObject for AI settings - accessed via GetUnmutableDefaults
//////////////////////////////////////////////////////////////////////////

UCLASS(config = Game, defaultconfig)
class COLONYBUILDER_API UColonyAISettings : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, config, Category = "AI", meta = (ClampMin = "0", UIMin = "0"))
	float PlanTickInterval;

	/** Static stages are stages that are ran whenever the plan is ticked - they act as setup functions, e.g FindWork, FindHouse, etc etc*/
	/** They do not get popped off the stack of stages in a plan*/
	UPROPERTY(EditAnywhere, config, Category = "Stages")
	TArray<TSubclassOf<UStage>> StaticStages;
};
