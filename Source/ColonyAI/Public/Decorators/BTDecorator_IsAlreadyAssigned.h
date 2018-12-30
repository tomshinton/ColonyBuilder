// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsAlreadyAssigned.generated.h"

//////////////////////////////////////////////////////////////////////////
// Is this controller already assigned to a Construction Project?
//////////////////////////////////////////////////////////////////////////

UCLASS()
class COLONYAI_API UBTDecorator_IsAlreadyAssigned : public UBTDecorator
{
	GENERATED_BODY()

	UBTDecorator_IsAlreadyAssigned();
		
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	virtual FString GetStaticDescription() const override;

};
