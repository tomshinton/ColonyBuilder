// ColonyBuilder Project, personal project by Tom Shinton

#include "ColonyCheatManager.h"
#include <EngineUtils.h>

#include "Utils/Libraries/ManagerUtils.h"

void UColonyCheatManager::ToggleAIReadout(const bool ShowReadout)
{
	FTimerHandle AIReadoutHandle;
	FTimerDelegate AIReadoutCallback;

	AIReadoutCallback.BindLambda([this]()
	{
		int32 VillagerCount = 0;
		int32 PlanCount = 0;
		int32 StageCount = 0;
		int32 TotalStageCount = 0;

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
				TotalStageCount++;

				if (Stage->GetIsActive())
				{
					StageCount++;
				}
			}
		}

		GEngine->AddOnScreenDebugMessage(4, 1, FColor::White, FString::Printf(TEXT("Total Stages: %i"), TotalStageCount));
		GEngine->AddOnScreenDebugMessage(1, 1, FColor::White, FString::Printf(TEXT("Active Stages: %i"), StageCount));
		GEngine->AddOnScreenDebugMessage(2, 1, FColor::White, FString::Printf(TEXT("Plans: %i"), PlanCount));
		GEngine->AddOnScreenDebugMessage(3, 1, FColor::White, FString::Printf(TEXT("Villagers: %i"), VillagerCount));

		if (StageCount > PlanCount)
		{
			const FString WarningReadout = "More stages active than plans! " + FString::FromInt(StageCount) + " Active Stages over " + FString::FromInt(PlanCount) + " plans!";
			GEngine->AddOnScreenDebugMessage(4, 1, FColorList::OrangeRed, FString::Printf(TEXT("%s"), *WarningReadout));
		}

		GEngine->AddOnScreenDebugMessage(5, 1, FColor::White, FString::Printf(TEXT("Advances queued: %i"), AdvanceQueue));
	});

	if (UWorld* World = GetWorld())
	{
		if (ShowReadout)
		{
			World->GetTimerManager().SetTimer(AIReadoutHandle, AIReadoutCallback, 0.1f, true, 0.f);
		}
		else
		{
			World->GetTimerManager().ClearTimer(AIReadoutHandle);
		}
	}
}

void UColonyCheatManager::BuildActions(const bool ShowReadout)
{
	FTimerHandle BuildActionsHandle;
	FTimerDelegate BuildActionsCallback;

	BuildActionsCallback.BindLambda([]()
	{
		int32 NumBuildActions = 0;
		for (TObjectIterator<UBuildAction> Itr; Itr; ++Itr)
		{
			if (UBuildAction* Stage = *Itr)
			{
				NumBuildActions++;
			}
		}

		GEngine->AddOnScreenDebugMessage(1, 1, FColor::White, FString::Printf(TEXT("Num spawned BuildActions: %i"), NumBuildActions));
	});

	if (UWorld* World = GetWorld())
	{
		if (ShowReadout)
		{
			World->GetTimerManager().SetTimer(BuildActionsHandle, BuildActionsCallback, 0.1f, true, 0.f);
		}
		else
		{
			World->GetTimerManager().ClearTimer(BuildActionsHandle);
		}
	}
}