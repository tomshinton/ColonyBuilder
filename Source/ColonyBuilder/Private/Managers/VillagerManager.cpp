// ColonyBuilder Project, personal project by Tom Shinton

#include "VillagerManager.h"
#include "BaseVillager.h"
#include <AIController.h>
#include "BehaviorTree/BlackboardComponent.h"
#include <GameFramework/Pawn.h>
#include <ConstructorHelpers.h>

void UVillagerManager::CreateVillagerFromSavedata(FVillagerSaveData& Savedata)
{
	if (UWorld* World = GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		if (ABaseVillager* NewVillager = World->SpawnActor<ABaseVillager>(Savedata.PawnClass, Savedata.VillagerTransform, SpawnParams))
		{
			NewVillager->LoadVillagerSaveData(Savedata);
			RegisterNewVillager(NewVillager);
		}
	}
}

void UVillagerManager::CreateVillagerAtLocation(const FVector Location)
{
	if (UWorld* World = GetWorld())
	{
		if (BaseVillagerClass)
		{
			FActorSpawnParameters SpawnParams;

			if (ABaseVillager* NewVillager = World->SpawnActor<ABaseVillager>(BaseVillagerClass, Location, FRotator::ZeroRotator, SpawnParams))
			{
				NewVillager->VillagerID = FGuid::NewGuid();
				RegisterNewVillager(NewVillager);
			}
		}
	}
}

void UVillagerManager::RegisterNewVillager(ABaseVillager* InNewVillager)
{
	SpawnedVillagers.Add(InNewVillager);
}

