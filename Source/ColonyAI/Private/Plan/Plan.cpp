// ColonyBuilder Project, personal project by Tom Shinton

#include "Plan.h"
#include "Plan/Stage.h"
#include "Settings/ColonyAISettings.h"
#include "VillagerManager.h"
#include "Utils/Libraries/ManagerUtils.h"

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
				CachedVillagerManager = Manager;
				AdvancePtr = [this](){ Advance(); };
			}
		}
	}
}

bool UPlan::CanAdvance() const
{
	return !CurrentStageInstance && (CurrentPlan.Num() > 0 || Plans.Num() > 0);
}

void UPlan::QueueAdvance()
{
	if (CachedVillagerManager)
	{
		CachedVillagerManager->PushAdvance(AdvancePtr);
	}
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

	if (CurrentStageInstance)
	{
		CurrentStageInstance->OnStageCompleted.RemoveAll(this);
		CurrentStageInstance = nullptr;
	}

	if (CurrentPlan.Num() <= 0 && Plans.Num() <= 0)
	{
		IsPlanActive = false;
		UE_LOG(PlanLog, Log, TEXT("Plan Over"));
	}
	else
	{
		if (CurrentPlan.Num() > 0)
		{
			CreateCurrentStageInstance(CurrentPlan[0]);

			CurrentStageInstance->OnStageCompleted.AddDynamic(this, &UPlan::Advance);
			CurrentStageInstance->Start();

			IsPlanActive = true;
		}
		else
		{
			if (Plans.Num() > 0)
			{
				CurrentPlan = Plans[0];
				Plans.RemoveAt(0);
			}
			QueueAdvance();
			return;
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
