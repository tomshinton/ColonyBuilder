// ColonyBuilder Project, personal project by Tom Shinton

#include "Task_Ungarrison.h"
#include "VillagerController.h"
#include "BaseVillager.h"
#include "ConstructionManager.h"
#include "Utils/Libraries/ManagerUtils.h"

EBTNodeResult::Type UTask_Ungarrison::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AVillagerController* OwningVillager = Cast<AVillagerController>(OwnerComp.GetAIOwner()))
	{
		if (ABaseVillager* OwningVillagerPawn = Cast<ABaseVillager>(OwningVillager->GetPawn()))
		{
			if (UConstructionManager* ConstructionManager = GetManager<UConstructionManager>(OwningVillagerPawn))
			{
				if (ConstructionManager->IsPawnGarrisoned(OwningVillagerPawn))
				{
					ConstructionManager->UngarrisonPawn(OwningVillagerPawn);
				}
			}
		}
	}

	return EBTNodeResult::Succeeded;
}
