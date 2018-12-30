// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsGarrisoned.generated.h"

//////////////////////////////////////////////////////////////////////////
// Is this pawn current registered with any garrison point?
//////////////////////////////////////////////////////////////////////////

UCLASS()
class COLONYAI_API UBTDecorator_IsGarrisoned : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_IsGarrisoned();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	virtual FString GetStaticDescription() const override;
};
