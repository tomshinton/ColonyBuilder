// ColonyBuilder Project, personal project by Tom Shinton

#include "BaseVillager.h"
#include "Controllers/VillagerController.h"

DEFINE_LOG_CATEGORY(VillagerLog);

// Sets default values
ABaseVillager::ABaseVillager()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AVillagerController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ABaseVillager::BeginPlay()
{
	Plan = NewObject<UPlan>(this, TEXT("VillagerPlan"));
}

FVillagerSaveData ABaseVillager::GetSaveData()
{
	FVillagerLocationData LocationData = FVillagerLocationData(ResidenceID, WorkplaceID);
	return FVillagerSaveData(VillagerID, GetClass(), GetTransform(), LocationData, Profession);
}

void ABaseVillager::LoadVillagerSaveData(const FVillagerSaveData& InData)
{
	VillagerID = InData.VillagerID;
	ResidenceID = InData.ImportantLocations.ResidenceID;
	WorkplaceID = InData.ImportantLocations.WorkplaceID;

	Profession = InData.Profession;
}

void ABaseVillager::OnReceiveHover()
{

}

void ABaseVillager::OnEndHover()
{

}

void ABaseVillager::OnSelect()
{
	UE_LOG(VillagerLog, Log, TEXT("Villager ID: %s"), *VillagerID.ToString());
	UE_LOG(VillagerLog, Log, TEXT("Villager Workplace ID: %s"), WorkplaceID.IsValid() ? *WorkplaceID.ToString() : TEXT("No workplace"));
	UE_LOG(VillagerLog, Log, TEXT("Villager Residence ID: %s"), ResidenceID.IsValid() ? *ResidenceID.ToString() : TEXT("No residence"));
}

void ABaseVillager::OnEndSelect()
{

}
