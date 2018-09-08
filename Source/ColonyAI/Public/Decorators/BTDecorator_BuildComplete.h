// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_BuildComplete.generated.h"

//////////////////////////////////////////////////////////////////////////
// Is this controller's assigned construction project done?
//////////////////////////////////////////////////////////////////////////

UCLASS()
class COLONYAI_API UBTDecorator_BuildComplete : public UBTDecorator
{
	GENERATED_BODY()

	UBTDecorator_BuildComplete();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	virtual FString GetStaticDescription() const override;

};
