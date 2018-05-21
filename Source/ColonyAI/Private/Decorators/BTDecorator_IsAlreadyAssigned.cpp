// ColonyBuilder Project, personal project by Tom Shinton

#include "BTDecorator_IsAlreadyAssigned.h"
#include "ColonyInstance.h"
#include "ConstructionManager.h"
#include "VillagerController.h"
#include "Kismet/GameplayStatics.h"

UBTDecorator_IsAlreadyAssigned::UBTDecorator_IsAlreadyAssigned()
{
	NodeName = "Is Controller Already Assigned To Job?";
}

bool UBTDecorator_IsAlreadyAssigned::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (UColonyInstance* GameInst = Cast<UColonyInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		if (UConstructionManager* ConstructionManager = GameInst->GetManager<UConstructionManager>())
		{
			AVillagerController* OwningVillager = Cast <AVillagerController>(OwnerComp.GetAIOwner());
			return ConstructionManager->IsControllerAlreadyRegistered(OwningVillager);
		}
	}

	return false;
}

FString UBTDecorator_IsAlreadyAssigned::GetStaticDescription() const
{
	return "Is Controller Already Registered?";
}
