// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_FindWorkplace.generated.h"

//////////////////////////////////////////////////////////////////////////
// Try and find a valid workplace for this pawn - queries the Construction Manager primarily
//////////////////////////////////////////////////////////////////////////

UCLASS()
class COLONYAI_API UTask_FindWorkplace : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
