// ColonyBuilder Project, personal project by Tom Shinton

#include "Stage.h"
#include "ColonyAISettings.h"

DEFINE_LOG_CATEGORY_STATIC(StageLog, Log, All);

UStage::UStage()
	: IsActive(true)
	, IsTickEnabled(false)
	, TickInterval(0.f)
{

}

void UStage::PostInitProperties()
{
	Super::PostInitProperties();

	if (IsTickEnabled && !FMath::IsNearlyZero(TickInterval))
	{
		if (UWorld* World = GetWorld())
		{
			FTimerDelegate TickDelegate;
			TickDelegate.BindUFunction(this, FName("OnStageTick"), TickInterval);

			World->GetTimerManager().SetTimer(TickStageHandle, TickDelegate, TickInterval, true, 0.f);
		}
	}
}

void UStage::OnStageTick_Implementation(const float DeltaTime)
{
	UE_LOG(StageLog, Log, TEXT("Stage Tick"));
}

void UStage::Start_Implementation(){}

void UStage::FinishExecute()
{
	OnStageCompleted.Broadcast();

	IsActive = false;

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TickStageHandle);
	}
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
