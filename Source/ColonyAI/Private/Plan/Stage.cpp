// ColonyBuilder Project, personal project by Tom Shinton

#include "Stage.h"

DEFINE_LOG_CATEGORY_STATIC(StageLog, Log, All);

UStage::UStage()
	: IsActive(true)
{

}

void UStage::OnPlanTick_Implementation(const float DeltaTime)
{
	UE_LOG(StageLog, Log, TEXT("Stage Tick"));
}

void UStage::Start_Implementation(){}

void UStage::FinishExecute()
{
	OnStageCompleted.Broadcast();

	IsActive = false;
}

void UStage::StageCallback(const FString String, const float Duration, const bool PrintToScreen)
{
	if (!String.IsEmpty())
	{
		UE_LOG(StageLog, Log, TEXT("%s"), *String);

		if (PrintToScreen)
		{
			GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::White, String);
		}
	}
}

class UWorld* UStage::GetWorld() const
{
	if (UObject* PlanOuter = GetOuter())
	{
		if (UObject* VillagerOuter = PlanOuter->GetOuter())
		{
			return VillagerOuter->GetWorld();
		}
	}

	return nullptr;
}
