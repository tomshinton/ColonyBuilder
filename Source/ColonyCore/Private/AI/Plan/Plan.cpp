// ColonyBuilder Project, personal project by Tom Shinton

#include "Plan.h"

#include "Utils/Libraries/ManagerUtils.h"
#include <DrawDebugHelpers.h>

#include "Stage.h"
#include "Core/Settings/ColonyAISettings.h"
#include "Managers/VillagerManager/VillagerManager.h"

DEFINE_LOG_CATEGORY_STATIC(PlanLog, Log, All);

DECLARE_CYCLE_STAT(TEXT("PlanAI AdvanceStage"), STAT_PlanAI_AdvanceStage, STATGROUP_PlanAI);
DECLARE_CYCLE_STAT(TEXT("PlanAI CreateStage"), STAT_PlanAI_CreateStage, STATGROUP_PlanAI);
DECLARE_CYCLE_STAT(TEXT("PlanAI CanAdvanceCheck"), STAT_PlanAI_CanAdvanceCheck, STATGROUP_PlanAI);

UPlan::UPlan()
	: IsPlanActive(false)
{}

void UPlan::PostInitProperties()
{
	Super::PostInitProperties();

	if (!CachedVillagerManager)
	{
		if (UWorld* World = GetOuter()->GetWorld())
		{
			if (UVillagerManager* Manager = GetManager<UVillagerManager>(World))
			{
				AdvancePtr = [&]() { Advance(); };
				CachedVillagerManager = Manager;
			}

			if (UColonyAISettings* AISettings = GetMutableDefault<UColonyAISettings>())
			{
				if (AISettings->RecusiveStages.Num() > 0 && !FMath::IsNearlyZero(AISettings->RecursivePlanFrequency))
				{
					RecursiveStages = AISettings->RecusiveStages;

					FTimerDelegate RecursiveStageCallback;
					FTimerHandle Handle;
					RecursiveStageCallback.BindLambda(
						[&]
					{
						if (AActor* OuterActor = Cast<AActor>(GetOuter()))
						{
							DrawDebugString(GetOuter()->GetWorld(), FVector(0,0,0), TEXT("Ambient plan pushed"), OuterActor, FColorList::HunterGreen, 1.f);
						}

						PushPlan(RecursiveStages);
					});

					World->GetTimerManager().SetTimer(Handle, RecursiveStageCallback, AISettings->RecursivePlanFrequency, true, 0.f);
				}
			}
		}
	}
}

bool UPlan::CanAdvance() const
{
	return !CurrentStageInstance && (CurrentPlan.Num() > 0 || Plans.Num() > 0);
}

void UPlan::ClearCurrentStage()
{
	if (CurrentStageInstance)
	{
		CurrentStageInstance->OnStageCompleted.RemoveAll(this);
		CurrentStageInstance->OnStageAborted.RemoveAll(this);

		if (UWorld* World = GetOuter()->GetWorld())
		{
			CurrentStageInstance->ConditionalBeginDestroy();
		}

		CurrentStageInstance = nullptr;
	}
}

void UPlan::QueueAdvance()
{
	if (CachedVillagerManager)
	{
		CachedVillagerManager->PushAdvance(AdvancePtr);
	}
}

void UPlan::AbortStage(const EStageAbortReason OnStageAborted)
{
	ClearCurrentStage();

	CurrentPlan.Empty();

	QueueAdvance();
}

void UPlan::PushPlan(TArray<TSubclassOf<UStage>> InPlan)
{
	Plans.Add(InPlan);

	if (CanAdvance())
	{
		QueueAdvance();
	}
}

void UPlan::Advance()
{
	SCOPE_CYCLE_COUNTER(STAT_PlanAI_AdvanceStage);

	ClearCurrentStage();

	if (CurrentPlan.Num() <= 0 && Plans.Num() <= 0)
	{
		IsPlanActive = false;
		UE_LOG(PlanLog, Log, TEXT("Plan Over"));
	}
	else
	{
		if (CurrentPlan.Num() <= 0)
		{
			if (Plans.Num() > 0)
			{
				CurrentPlan = Plans[0];
				Plans.RemoveAt(0);
			}
		}

		//Do we have any new plans to carry on with, post swap?
		if (CurrentPlan.Num() > 0)
		{
			CreateCurrentStageInstance(CurrentPlan[0]);

			CurrentStageInstance->OnStageCompleted.AddDynamic(this, &UPlan::QueueAdvance);
			CurrentStageInstance->OnStageAborted.AddDynamic(this, &UPlan::AbortStage);
			CurrentStageInstance->Start();

			IsPlanActive = true;
		}
	}
}

void UPlan::CreateCurrentStageInstance(TSubclassOf<UStage> InStageTemplate)
{
	SCOPE_CYCLE_COUNTER(STAT_PlanAI_CreateStage);

	CurrentStageInstance = NewObject<UStage>(this, InStageTemplate);

	if (CurrentStageInstance)
	{
		CurrentPlan.RemoveAt(0);
	}
}
