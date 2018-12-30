// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "VillagerController.generated.h"

class UProfessionBase;

UCLASS()
class COLONYAI_API AVillagerController : public AAIController
{
	GENERATED_BODY()

	AVillagerController();
public:
	UBlackboardComponent* BlackboardComp;
	UBehaviorTreeComponent* BehaviourTreeComp;

private:
	UBehaviorTree* BehaviourTree;
	UBlackboardData* Blackboard;
	
public:
	virtual void Possess(APawn* InPawn) override;

};
