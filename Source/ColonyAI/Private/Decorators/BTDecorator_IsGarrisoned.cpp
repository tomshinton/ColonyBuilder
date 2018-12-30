// ColonyBuilder Project, personal project by Tom Shinton

#include "BTDecorator_IsGarrisoned.h"
#include "ConstructionManager.h"
#include "Utils/Libraries/ManagerUtils.h"

UBTDecorator_IsGarrisoned::UBTDecorator_IsGarrisoned()
{
	NodeName = "Is Pawn Garrisoned?";
}

bool UBTDecorator_IsGarrisoned::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (UWorld* World = GetWorld())
	{
		if (UConstructionManager* ConstructionManager = GetManager<UConstructionManager>(World))
		{
			if (AController* OwningController = Cast<AController>(OwnerComp.GetOwner()))
			{
				if (ABaseVillager* OwningPawn = Cast<ABaseVillager>(OwningController->GetPawn()))
				{
					return ConstructionManager->IsPawnGarrisoned(OwningPawn);
				}
			}
		}
	}

	return false;
}

FString UBTDecorator_IsGarrisoned::GetStaticDescription() const
{
	return TEXT("Is this controller's pawn garrisoned?");
}


