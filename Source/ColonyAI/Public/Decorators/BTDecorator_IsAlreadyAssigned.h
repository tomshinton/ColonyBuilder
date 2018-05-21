// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsAlreadyAssigned.generated.h"

/**
 * 
 */
UCLASS()
class COLONYAI_API UBTDecorator_IsAlreadyAssigned : public UBTDecorator
{
	GENERATED_BODY()
	
		
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
