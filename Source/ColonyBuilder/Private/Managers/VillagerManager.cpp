// ColonyBuilder Project, personal project by Tom Shinton

#include "VillagerManager.h"
#include "BaseVillager.h"
#include <AIController.h>
#include "BehaviorTree/BlackboardComponent.h"
#include <GameFramework/Pawn.h>
#include <ConstructorHelpers.h>
#include "ColonyAISettings.h"


void UVillagerManager::PostInitProperties()
{
	Super::PostInitProperties();

	if (UWorld* World = GetWorld())
	{
		if (UColonyAISettings* AISettings = GetMutableDefault<UColonyAISettings>())
		{
			World->GetTimerManager().SetTimer(TickPlanHandle, this, &UVillagerManager::TickPlanAdvance, AISettings->PlanAdvanceInterval, true, 0.f);
		}
	}
}

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

void UVillagerManager::PushAdvance(TFunction<void()> InFunc)
{
	AdvanceFuncArray.Add(InFunc);

	if (UWorld* World = GetWorld())
	{
		FTimerManager& TimerRef = World->GetTimerManager();
		if (!TimerRef.IsTimerActive(TickPlanHandle))
		{
			//TimerRef.UnPauseTimer(TickPlanHandle);
		}
	}
}

void UVillagerManager::TickPlanAdvance()
{
	if (AdvanceFuncArray.Num() > 0)
	{
		AdvanceFuncArray[0]();
		AdvanceFuncArray.RemoveAt(0);
	}
	else
	{
		if (UWorld* World = GetWorld())
		{
			//World->GetTimerManager().PauseTimer(TickPlanHandle);
		}
	}
}
