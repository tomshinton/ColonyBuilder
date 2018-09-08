// ColonyBuilder Project, personal project by Tom Shinton

#include "BTDecorator_IsThisProfession.h"
#include "Professions.h"
#include "VillagerController.h"
#include "BaseVillager.h"

UBTDecorator_IsThisProfession::UBTDecorator_IsThisProfession()
{
	NodeName = "Is This Profession";
}

bool UBTDecorator_IsThisProfession::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (AVillagerController* OwningVillagerController = Cast<AVillagerController>(OwnerComp.GetAIOwner()))
	{
		if(ABaseVillager* OwningVillagerPawn = Cast<ABaseVillager>(OwningVillagerController))
		{
			return OwningVillagerPawn->GetProfession() == Profession;
		}
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
