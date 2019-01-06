// ColonyBuilder Project, personal project by Tom Shinton

#include "ColonyCheatManager.h"
#include <EngineUtils.h>
#include "BaseVillager.h"
#include "Plan.h"
#include "Stage.h"
#include "VillagerManager.h"
#include "Utils/Libraries/ManagerUtils.h"

void UColonyCheatManager::ToggleAIReadout()
{
	if (UWorld* World = GetWorld())
	{
		if (AIReadoutActive)
		{
			World->GetTimerManager().ClearTimer(AIReadoutTimerHandle);
			AIReadoutActive = false;
		}
		else
		{
			World->GetTimerManager().SetTimer(AIReadoutTimerHandle, this, &UColonyCheatManager::ShowAIReadout, 0.1f, true, 0.f);
			AIReadoutActive = true;
		}
	}
}

void UColonyCheatManager::ShowAIReadout() const
{
	int32 VillagerCount = 0;
	int32 PlanCount = 0;
	int32 StageCount = 0;

	const int32 AdvanceQueue = GetManager<UVillagerManager>(GetWorld())->GetAdvanceQueueNum();

	for (TActorIterator<ABaseVillager> Itr(GetWorld()); Itr; ++Itr)
	{
		if (ABaseVillager* Villager = *Itr)
		{
			VillagerCount++;
		}
	}

	for (TObjectIterator<UPlan> Itr; Itr; ++Itr)
	{
		if (UPlan* Plan = *Itr)
		{
			PlanCount++;
		}
	}

	for (TObjectIterator<UStage> Itr; Itr; ++Itr)
	{
		if (UStage* Stage = *Itr)
		{
			if (Stage->GetIsActive())
			{
				StageCount++;
			}
		}
	}

	GEngine->AddOnScreenDebugMessage(1, 1, FColor::White, FString::Printf(TEXT("Active Stages: %i"), StageCount));
	GEngine->AddOnScreenDebugMessage(2, 1, FColor::White, FString::Printf(TEXT("Plans: %i"), PlanCount));
	GEngine->AddOnScreenDebugMessage(3, 1, FColor::White, FString::Printf(TEXT("Villagers: %i"), VillagerCount));

	if (StageCount > PlanCount)
	{
		const FString WarningReadout = "More stages active than plans! " + FString::FromInt(StageCount) + " Active Stages over " + FString::FromInt(PlanCount) + " plans!";
		GEngine->AddOnScreenDebugMessage(4, 1, FColorList::OrangeRed, FString::Printf(TEXT("%s"), *WarningReadout));
	}

	GEngine->AddOnScreenDebugMessage(5, 1, FColor::White, FString::Printf(TEXT("Advances queued: %i"), AdvanceQueue));
}
