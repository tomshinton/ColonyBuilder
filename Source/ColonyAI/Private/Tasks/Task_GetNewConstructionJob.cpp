// ColonyBuilder Project, personal project by Tom Shinton

#include "Task_GetNewConstructionJob.h"
#include "VillagerController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ColonyInstance.h"
#include "ConstructionManager.h"
#include "ConstructionComponent.h"
#include "BTNodeUtils.h"
#include "Utils/Libraries/ManagerUtils.h"

const FName UTask_GetNewConstructionJob::MoveToLocationKey(TEXT("MoveToLocation"));

UTask_GetNewConstructionJob::UTask_GetNewConstructionJob()
{}

EBTNodeResult::Type UTask_GetNewConstructionJob::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AVillagerController* OwningVillager = Cast<AVillagerController>(OwnerComp.GetAIOwner()))
	{
		if (UBlackboardComponent* OwningBlackboard = OwningVillager->GetBlackboardComponent())
		{
			if (UConstructionManager* ConstructionManager = GetManager<UConstructionManager>(GetWorld()))
			{
				TWeakObjectPtr<UConstructionSiteComponent> FoundSite = nullptr;

				UConstructionComponent* FoundConstruction = ConstructionManager->ProcessNewConstructionRequest(OwningVillager, FoundSite);

				if (FoundSite.IsValid())
				{
					const FVector FoundBuildLocation = GetRandomNavigablePointInVolume(FoundSite->GetComponentLocation(), FoundSite->CollisionComponent->GetScaledBoxExtent(), this);

					if (FoundConstruction)
					{
						OwningBlackboard->SetValueAsVector(UTask_GetNewConstructionJob::MoveToLocationKey, FoundBuildLocation);
						return EBTNodeResult::Succeeded;
					}
				}

				return EBTNodeResult::Failed;
			}
		}
	}

	return EBTNodeResult::Failed;
}
