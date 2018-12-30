// ColonyBuilder Project, personal project by Tom Shinton

#include "BTDecorator_HasWorkplace.h"
#include "Professions.h"
#include "VillagerController.h"
#include "ColonyInstance.h"
#include <Kismet/GameplayStatics.h>
#include "BaseVillager.h"

UBTDecorator_HasWorkplace::UBTDecorator_HasWorkplace()
{
	NodeName = "Has a valid Workplace";
}

bool UBTDecorator_HasWorkplace::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (AVillagerController* OwningVillager = Cast <AVillagerController>(OwnerComp.GetAIOwner()))
	{
		if (ABaseVillager* VillagerPawn = Cast<ABaseVillager>(OwningVillager->GetPawn()))
		{
			const bool HasValidWorkspace = VillagerPawn->WorkplaceID.IsValid();

			return HasValidWorkspace;
		}
	}

	return false;
}

FString UBTDecorator_HasWorkplace::GetStaticDescription() const
{
	const FString StaticDescription = "Does this pawn have a valid place to work?";

	return StaticDescription;
}
