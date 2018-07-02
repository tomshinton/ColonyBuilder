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

	TSubclassOf<UProfessionBase> GetProfession() { return Profession; }

	bool IsPawnEncroaching() { return PawnIsEncroaching; }

private:
	UBehaviorTree* BehaviourTree;
	UBlackboardData* Blackboard;

	bool PawnIsEncroaching;

	TSubclassOf<UProfessionBase> Profession;
	
public:
	virtual void Possess(APawn* InPawn) override;

};
