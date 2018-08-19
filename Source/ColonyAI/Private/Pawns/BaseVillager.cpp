// ColonyBuilder Project, personal project by Tom Shinton

#include "BaseVillager.h"
#include "Controllers/VillagerController.h"
#include "PackedBlackboard.h"

// Sets default values
ABaseVillager::ABaseVillager()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AVillagerController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

FVillagerSaveData ABaseVillager::GetSaveData()
{
	FPackedBlackboard PackedBlackboard = FPackedBlackboard();

	if (AVillagerController* Controller = Cast<AVillagerController>(GetController()))
	{
		PackedBlackboard.Pack(Controller->BlackboardComp);
	}

	return FVillagerSaveData(GetClass(), GetTransform(), PackedBlackboard);
}

void ABaseVillager::LoadVillagerSaveData(const FVillagerSaveData& InData)
{

}
