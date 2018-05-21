// ColonyBuilder Project, personal project by Tom Shinton

#include "Task_GetNewConstructionJob.h"
#include "VillagerController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ColonyInstance.h"
#include "ConstructionManager.h"
#include "ConstructionComponent.h"

const FName UTask_GetNewConstructionJob::TargetLocationKey(TEXT("MoveToLocation"));

UTask_GetNewConstructionJob::UTask_GetNewConstructionJob()
{}

EBTNodeResult::Type UTask_GetNewConstructionJob::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AVillagerController* OwningVillager = Cast<AVillagerController>(OwnerComp.GetAIOwner()))
	{
		if (UBlackboardComponent* OwningBlackboard = OwningVillager->GetBlackboardComponent())
		{
			if (UColonyInstance* ColonyInstance = Cast<UColonyInstance>(GetWorld()->GetGameInstance()))
			{
				if(UConstructionManager* ConstructionManager = ColonyInstance->GetManager<UConstructionManager>())
				{
					FVector SiteLocation;
					UConstructionComponent* FoundConstruction = ConstructionManager->ProcessNewConstructionRequest(OwningVillager, SiteLocation);

					if (FoundConstruction)
					{
						OwningBlackboard->SetValueAsVector(UTask_GetNewConstructionJob::TargetLocationKey, SiteLocation);
						return EBTNodeResult::Succeeded;
					}
				}
			}
		}
	}

	return EBTNodeResult::Failed;

}
