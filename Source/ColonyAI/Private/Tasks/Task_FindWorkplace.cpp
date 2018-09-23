// ColonyBuilder Project, personal project by Tom Shinton

#include "Task_FindWorkplace.h"
#include "ColonyInstance.h"
#include "ConstructionManager.h"
#include "VillagerController.h"
#include "BaseVillager.h"

EBTNodeResult::Type UTask_FindWorkplace::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AVillagerController* OwningVillager = Cast<AVillagerController>(OwnerComp.GetAIOwner()))
	{
		if (ABaseVillager* OwningVillagerPawn = Cast<ABaseVillager>(OwningVillager->GetPawn()))
		{
			if (UColonyInstance* ColonyInstance = Cast<UColonyInstance>(GetWorld()->GetGameInstance()))
			{
				if (UConstructionManager* ConstructionManager = ColonyInstance->GetManager<UConstructionManager>())
				{
					const bool PawnHasBeenAssigned = ConstructionManager->AssignPawnToWorkplace(OwningVillagerPawn);

					return PawnHasBeenAssigned ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
				}
			}
		}
	}

	return EBTNodeResult::Failed;
}
