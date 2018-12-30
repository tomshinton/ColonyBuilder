// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BTDecorator_IsThisProfession.generated.h"

class UProfessionBase;

UCLASS()
class COLONYAI_API UBTDecorator_IsThisProfession : public UBTDecorator
{
	GENERATED_BODY()

	UBTDecorator_IsThisProfession();
	
public:

	UPROPERTY(EditAnywhere, Category = "Decorator")
	TSubclassOf<UProfessionBase> Profession;

private:
	virtual FString GetStaticDescription() const override;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
