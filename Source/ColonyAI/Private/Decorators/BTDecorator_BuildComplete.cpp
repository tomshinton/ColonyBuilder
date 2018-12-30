// ColonyBuilder Project, personal project by Tom Shinton

#include "BTDecorator_BuildComplete.h"
#include "ColonyInstance.h"
#include "ConstructionManager.h"
#include "VillagerController.h"
#include "Utils/Libraries/ManagerUtils.h"

UBTDecorator_BuildComplete::UBTDecorator_BuildComplete()
{
	NodeName = "Is Controller Ready to leave construction site?";
}

bool UBTDecorator_BuildComplete::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (UConstructionManager* ConstructionManager = GetManager<UConstructionManager>(GetWorld()))
	{
		AVillagerController* OwningVillager = Cast <AVillagerController>(OwnerComp.GetAIOwner());
		return ConstructionManager->IsControllerOnPendingFinishComponent(OwningVillager);
	}

	return false;
}

FString UBTDecorator_BuildComplete::GetStaticDescription() const
{
	return "Can Controller move on from current project?";
}
