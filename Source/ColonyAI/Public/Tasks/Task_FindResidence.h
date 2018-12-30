// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_FindResidence.generated.h"

//////////////////////////////////////////////////////////////////////////
// Try and find a valid residence for this pawn - queries the Construction Manager primarily
//////////////////////////////////////////////////////////////////////////

UCLASS()
class COLONYAI_API UTask_FindResidence : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
