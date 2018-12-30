// ColonyBuilder Project, personal project by Tom Shinton

#include "BTDecorator_IsAlreadyAssigned.h"
#include "ColonyInstance.h"
#include "ConstructionManager.h"
#include "VillagerController.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/Libraries/ManagerUtils.h"

UBTDecorator_IsAlreadyAssigned::UBTDecorator_IsAlreadyAssigned()
{
	NodeName = "Is Controller Already Assigned To Job?";
}

bool UBTDecorator_IsAlreadyAssigned::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (UConstructionManager* ConstructionManager = GetManager<UConstructionManager>(GetWorld()))
	{
		AVillagerController* OwningVillager = Cast <AVillagerController>(OwnerComp.GetAIOwner());
		const bool IsAssigned = ConstructionManager->IsControllerOnActiveComponent(OwningVillager) || ConstructionManager->IsControllerOnPendingFinishComponent(OwningVillager);
		return IsAssigned;
	}

	return false;
}

FString UBTDecorator_IsAlreadyAssigned::GetStaticDescription() const
{
	return "Is Controller Already Registered?";
}
