// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_Ungarrison.generated.h"

//////////////////////////////////////////////////////////////////////////
// Ungarrison this pawn - if they're not currently garrisoned, we'll just skip over this, no biggie, this is almost always a helper task for the cpp hookins
//////////////////////////////////////////////////////////////////////////

UCLASS()
class COLONYAI_API UTask_Ungarrison : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
