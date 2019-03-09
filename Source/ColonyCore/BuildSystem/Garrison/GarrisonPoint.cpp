// ColonyBuilder Project, personal project by Tom Shinton

#include "GarrisonPoint.h"
#include <Components/StaticMeshComponent.h>
#include <Components/BoxComponent.h>

DEFINE_LOG_CATEGORY_STATIC(GarrisonComponentLog, Log, All);

UGarrisonPoint::UGarrisonPoint()
	: MeshComponent(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh")))
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGarrisonPoint::Ungarrison(ABaseVillager* BaseVillager)
{
	GarrisonedVillagers.Remove(BaseVillager);
	BaseVillager->SetActorHiddenInGame(false);
}

void UGarrisonPoint::Garrison(ABaseVillager* BaseVillager)
{
	GarrisonedVillagers.Add(BaseVillager);
	BaseVillager->SetActorHiddenInGame(true);
}
