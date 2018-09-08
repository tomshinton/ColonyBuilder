// ColonyBuilder Project, personal project by Tom Shinton

#include "BTDecorator_HasResidence.h"
#include "VillagerController.h"
#include "ColonyInstance.h"
#include <Kismet/GameplayStatics.h>
#include "ConstructionManager.h"

UBTDecorator_HasResidence::UBTDecorator_HasResidence()
{
	NodeName = "Has Residence";
}

bool UBTDecorator_HasResidence::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (UColonyInstance* GameInst = Cast<UColonyInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		if (UConstructionManager* ConstructionManager = GameInst->GetManager<UConstructionManager>())
		{
			AVillagerController* OwningVillager = Cast <AVillagerController>(OwnerComp.GetAIOwner());

			if (ABaseVillager* VillagerPawn = Cast<ABaseVillager>(OwningVillager->GetPawn()))
			{
				return ConstructionManager->IsPawnRegistedAsResident(VillagerPawn).IsValid();
			}
		}
	}

	return false;
}


FString UBTDecorator_HasResidence::GetStaticDescription() const
{
	const FString StaticDescription = "Does this pawn have a valid place to live?";

	return StaticDescription;
}
