// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include <EnvironmentQuery/EnvQuery.h>
#include "Task_GetNewConstructionJob.generated.h"

/**
 * 
 */
UCLASS()
class COLONYAI_API UTask_GetNewConstructionJob : public UBTTaskNode
{
	GENERATED_BODY()

	UTask_GetNewConstructionJob();

protected:
	static const FName MoveToLocationKey;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
