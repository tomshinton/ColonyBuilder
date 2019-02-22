// ColonyBuilder Project, personal project by Tom Shinton

#include "FindWork.h"
#include "ConstructionManager.h"
#include "BaseVillager.h"
#include "Utils/Libraries/ManagerUtils.h"
#include "JournalUtils.h"

// NOTE: Ambient task, so must always finish execute, regardless of success/failure status
void UFindWork::OnStart()
{
	if (UJournalUtils::KeyExists(GetVillagerOuter()->GetJournal(), StaticKeys::WorkplaceID, EKeyType::Guid))
	{
		const FGuid KeyValue = UJournalUtils::GetKeyAsGuid(GetVillagerOuter()->GetJournal(), StaticKeys::WorkplaceID);

		if (KeyValue.IsValid())
		{
			FinishExecute();
			return;
		}
	}

	if (ABaseVillager* OwningVillagerPawn = Cast<ABaseVillager>(GetVillagerOuter()))
	{
		if (UConstructionManager* ConstructionManager = GetManager<UConstructionManager>(OwningVillagerPawn))
		{
			const FGuid AssignedWorkplace = ConstructionManager->AssignPawnToWorkplace(OwningVillagerPawn);

			if (AssignedWorkplace.IsValid())
			{
				UJournalUtils::SetKeyAsGuid(OwningVillagerPawn, StaticKeys::WorkplaceID, AssignedWorkplace);
			}
		}
	}

	FinishExecute();
}
