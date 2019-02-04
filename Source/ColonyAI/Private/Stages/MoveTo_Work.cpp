// ColonyBuilder Project, personal project by Tom Shinton

#include "MoveTo_Work.h"
#include "ConstructionManager.h"
#include "BuildableBase.h"
#include "JournalUtils.h"
#include "Utils/Libraries/ManagerUtils.h"
#include "GarrisonPoint.h"

UMoveTo_Work::UMoveTo_Work()
{
	ShouldGarrisonOnEnd = true;
}

void UMoveTo_Work::OnSetMoveToLocation()
{
	if (UJournalUtils::KeyExists(GetVillagerOuter()->GetJournal(), StaticKeys::WorkplaceID, EKeyType::Guid))
	{
		const FGuid WorkplaceID = UJournalUtils::GetKeyAsGuid(GetVillagerOuter()->GetJournal(), StaticKeys::WorkplaceID);

		if (UConstructionManager* ConstructionManager = GetManager<UConstructionManager>(GetVillagerOuter()->GetWorld()))
		{
			if (ABuildableBase* Workplace = ConstructionManager->GetBuildingFromId(WorkplaceID))
			{
				MoveToLocation = Workplace->GetGarrisonPoint().Get()->GetComponentLocation();
				TargetBuildingID = WorkplaceID;
			}
		}
	}
	else
	{
		AbortExecute(EStageAbortReason::Failure);
	}
}
