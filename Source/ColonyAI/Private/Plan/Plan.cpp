// ColonyBuilder Project, personal project by Tom Shinton

#include "Plan.h"
#include "Plan/Stage.h"
#include "Settings/ColonyAISettings.h"

DEFINE_LOG_CATEGORY_STATIC(PlanLog, Log, All);

DECLARE_CYCLE_STAT(TEXT("PlanAI Tick"), STAT_PlanAI_Tick, STATGROUP_PlanAI);
DECLARE_CYCLE_STAT(TEXT("PlanAI AdvanceStage"), STAT_PlanAI_AdvanceStage, STATGROUP_PlanAI);
DECLARE_CYCLE_STAT(TEXT("PlanAI CreateStage"), STAT_PlanAI_CreateStage, STATGROUP_PlanAI);
DECLARE_CYCLE_STAT(TEXT("PlanAI CanAdvanceCheck"), STAT_PlanAI_CanAdvanceCheck, STATGROUP_PlanAI);


UPlan::UPlan()
	: IsPlanActive(false)
{}

void UPlan::PostInitProperties()
{
	Super::PostInitProperties();

	if (UColonyAISettings* AISettings = GetMutableDefault<UColonyAISettings>())
	{
		if (UWorld* World = GetOuter()->GetWorld())
		{
			const float PlanTickInterval = AISettings->PlanTickInterval;

			FTimerDelegate PlanTickDelegate;
			PlanTickDelegate.BindUFunction(this, FName("TickPlan"), PlanTickInterval);

			World->GetTimerManager().SetTimer(PlanTickHandle, PlanTickDelegate, PlanTickInterval, true, 0.f);
		}
	}
}

void UPlan::TickPlan(const float DeltaTime)
{
	SCOPE_CYCLE_COUNTER(STAT_PlanAI_Tick);

	OnTick.Broadcast(DeltaTime);

	if (CanAdvance())
	{
		SCOPE_CYCLE_COUNTER(STAT_PlanAI_CanAdvanceCheck);
		
		Advance();
	}
}

bool UPlan::CanAdvance() const
{
	return !CurrentStageInstance && (CurrentPlan.Num() > 0 || Plans.Num() > 0);
	//return !IsPlanActive && (CurrentPlan.Num() >= || Plans.Num() <= 0);
}

void UPlan::PushPlan(TArray<TSubclassOf<UStage>> InPlan)
{
	Plans.Add(InPlan);
}

void UPlan::Advance()
{
	SCOPE_CYCLE_COUNTER(STAT_PlanAI_AdvanceStage);

	if (CurrentStageInstance)
	{
		CurrentStageInstance->OnStageCompleted.RemoveAll(this);
		OnTick.RemoveDynamic(CurrentStageInstance, &UStage::OnPlanTick);
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
			Advance();
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
		OnTick.AddDynamic(CurrentStageInstance, &UStage::OnPlanTick);
		CurrentPlan.RemoveAt(0);
	}
}
