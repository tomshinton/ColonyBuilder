// ColonyBuilder Project, personal project by Tom Shinton

#include "FindResidence.h"

#include "Utils/Libraries/ManagerUtils.h"

#include "AI/Pawns/BaseVillager.h"
#include "AI/Journal/JournalUtils.h"
#include "BuildSystem/Construction/Manager/ConstructionManager.h"

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
