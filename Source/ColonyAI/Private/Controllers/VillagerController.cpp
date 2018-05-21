// ColonyBuilder Project, personal project by Tom Shinton

#include "VillagerController.h"
#include "ConstructorHelpers.h"
#include "BaseVillager.h"
#include "Professions.h"

AVillagerController::AVillagerController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	BehaviourTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviourComp"));

	ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviourTreeRef(TEXT("BehaviorTree'/Game/AI/BT/BaseVillager/BT_BaseVillager.BT_BaseVillager'"));
	if (BehaviourTreeRef.Object)
	{
		BehaviourTree = BehaviourTreeRef.Object;
	}

	ConstructorHelpers::FObjectFinder<UBlackboardData> BlackboardDataRef(TEXT("BlackboardData'/Game/AI/BT/BaseVillager/BB_BaseVillager.BB_BaseVillager'"));
	if (BlackboardDataRef.Object)
	{
		Blackboard = BlackboardDataRef.Object;
	}

	Profession = UBuilder::StaticClass();
}

void AVillagerController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	RunBehaviorTree(BehaviourTree);
	UseBlackboard(Blackboard, BlackboardComp);

	BehaviourTreeComp->StartTree(*BehaviourTree);
}
