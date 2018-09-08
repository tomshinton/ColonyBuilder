// ColonyBuilder Project, personal project by Tom Shinton

#include "BTDecorator_HasWorkplace.h"
#include "Professions.h"
#include "VillagerController.h"

UBTDecorator_HasWorkplace::UBTDecorator_HasWorkplace()
{
	NodeName = "Has a valid Workplace";
}

bool UBTDecorator_HasWorkplace::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return false;
}

FString UBTDecorator_HasWorkplace::GetStaticDescription() const
{
	const FString StaticDescription = "Does this pawn have a valid place to work?";

	return StaticDescription;
}
