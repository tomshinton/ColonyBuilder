// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BTDecorator_HasWorkplace.generated.h"

UCLASS()
class COLONYAI_API UBTDecorator_HasWorkplace : public UBTDecorator
{
	GENERATED_BODY()

	UBTDecorator_HasWorkplace();
	
private:
	virtual FString GetStaticDescription() const override;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
