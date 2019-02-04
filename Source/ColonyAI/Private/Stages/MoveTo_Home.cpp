// ColonyBuilder Project, personal project by Tom Shinton

#include "MoveTo_Home.h"
#include "ConstructionManager.h"
#include "BuildableBase.h"
#include "JournalUtils.h"
#include "Utils/Libraries/ManagerUtils.h"
#include "GarrisonPoint.h"

UMoveTo_Home::UMoveTo_Home()
{
	ShouldGarrisonOnEnd = true;
}

void UMoveTo_Home::OnSetMoveToLocation()
{
	if (UJournalUtils::KeyExists(GetVillagerOuter()->GetJournal(), StaticKeys::ResidenceID, EKeyType::Guid))
	{
		const FGuid ResidenceID = UJournalUtils::GetKeyAsGuid(GetVillagerOuter()->GetJournal(), StaticKeys::ResidenceID);

		if (UConstructionManager* ConstructionManager = GetManager<UConstructionManager>(GetVillagerOuter()->GetWorld()))
		{
			if (ABuildableBase* Residence = ConstructionManager->GetBuildingFromId(ResidenceID))
			{
				MoveToLocation = Residence->GetGarrisonPoint().Get()->GetComponentLocation();	
				TargetBuildingID = ResidenceID;
			}
		}
	}
	else
	{
		AbortExecute(EStageAbortReason::Failure);
	}
}
