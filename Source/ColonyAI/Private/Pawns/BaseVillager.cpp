// ColonyBuilder Project, personal project by Tom Shinton

#include "BaseVillager.h"

#include <Components/CapsuleComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <Components/StaticMeshComponent.h>

DEFINE_LOG_CATEGORY(VillagerLog);

const float ABaseVillager::AgentRadius(50.f);
const float ABaseVillager::AgentHeight(100.f);

// Sets default values
ABaseVillager::ABaseVillager()
: Capsule(CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp")))
, SkelMeshComp(CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp")))
, StaticMeshComponent(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp")))
{
	PrimaryActorTick.bCanEverTick = false;

	Capsule->SetCapsuleHalfHeight(AgentHeight);
	Capsule->SetCapsuleRadius(AgentHeight);

	RootComponent = Capsule;

	SkelMeshComp->SetupAttachment(Capsule);
	StaticMeshComponent->SetupAttachment(Capsule);

	MoveSpeed = 10.f;
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
