// ColonyBuilder Project, personal project by Tom Shinton

#include "BaseVillager.h"
#include "Controllers/VillagerController.h"

// Sets default values
ABaseVillager::ABaseVillager()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AVillagerController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
}