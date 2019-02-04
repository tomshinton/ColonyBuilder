// ColonyBuilder Project, personal project by Tom Shinton

#include "FindResidence.h"
#include "ConstructionManager.h"
#include "BaseVillager.h"
#include "Utils/Libraries/ManagerUtils.h"
#include "JournalUtils.h"

// NOTE: Ambient task, so must always finish execute, regardless of success/failure status
void UFindResidence::OnStart()
{
	if (UJournalUtils::KeyExists(GetVillagerOuter()->GetJournal(), StaticKeys::ResidenceID, EKeyType::Guid))
	{
		const FGuid KeyValue = UJournalUtils::GetKeyAsGuid(GetVillagerOuter()->GetJournal(), StaticKeys::ResidenceID);

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
			const FGuid AssignedResidence = ConstructionManager->AssignPawnToResidence(OwningVillagerPawn);

			if (AssignedResidence.IsValid())
			{
				UJournalUtils::SetKeyAsGuid(OwningVillagerPawn, StaticKeys::ResidenceID, AssignedResidence);
			}
		}
	}

	FinishExecute();
}
