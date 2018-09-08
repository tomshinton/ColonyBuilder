// ColonyBuilder Project, personal project by Tom Shinton

#include "BTDecorator_BuildComplete.h"
#include "ColonyInstance.h"
#include <Kismet/GameplayStatics.h>
#include "ConstructionManager.h"
#include "VillagerController.h"

UBTDecorator_BuildComplete::UBTDecorator_BuildComplete()
{
	NodeName = "Is Controller Ready to leave construction site?";
}

bool UBTDecorator_BuildComplete::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (UColonyInstance* GameInst = Cast<UColonyInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		if (UConstructionManager* ConstructionManager = GameInst->GetManager<UConstructionManager>())
		{
			AVillagerController* OwningVillager = Cast <AVillagerController>(OwnerComp.GetAIOwner());
			return ConstructionManager->IsControllerOnPendingFinishComponent(OwningVillager);
		}
	}

	return false;
}

FString UBTDecorator_BuildComplete::GetStaticDescription() const
{
	return "Can Controller move on from current project?";
}
