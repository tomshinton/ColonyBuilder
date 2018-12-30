// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include <EnvironmentQuery/EnvQuery.h>
#include "Task_GetNewConstructionJob.generated.h"

//////////////////////////////////////////////////////////////////////////
// Get a new construction job.  This is just a ptr to a construction site that can hold a new Builder.  Does not include a MoveTo
//////////////////////////////////////////////////////////////////////////

UCLASS()
class COLONYAI_API UTask_GetNewConstructionJob : public UBTTaskNode
{
	GENERATED_BODY()

	UTask_GetNewConstructionJob();

protected:
	static const FName MoveToLocationKey;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
