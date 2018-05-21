// ColonyBuilder Project, personal project by Tom Shinton

#include "BTDecorator_IsThisProfession.h"
#include "Professions.h"
#include "VillagerController.h"

UBTDecorator_IsThisProfession::UBTDecorator_IsThisProfession()
{
	NodeName = "Is This Profession";
}

bool UBTDecorator_IsThisProfession::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AVillagerController* OwningVillagerController = Cast<AVillagerController>(OwnerComp.GetAIOwner());

	if (OwningVillagerController && OwningVillagerController->GetProfession() == Profession)
	{
		return true;
	}
	
	return false;
}

FString UBTDecorator_IsThisProfession::GetStaticDescription() const
{
	FString StaticDescription;

	if (Profession)
	{
		StaticDescription = "Is this controller a " + Profession->GetFName().ToString() + "?";

	}
	else
	{
		StaticDescription = "No profession filter set";
	}

	return StaticDescription;

}
