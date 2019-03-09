// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <UniquePtr.h>
#include "Plan.generated.h"

class UStage;
class UVillagerManager;

//////////////////////////////////////////////////////////////////////////
// Base Plan for AI FiniteStateMachine
//////////////////////////////////////////////////////////////////////////

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlanTick, const float, DeltaTime);

DECLARE_STATS_GROUP(TEXT("PlanAI"), STATGROUP_PlanAI, STATCAT_Advanced);

UENUM()
enum class EStageAbortReason : uint8
{
	Failure
};

UCLASS()
class COLONYCORE_API UPlan : public UObject
{
	GENERATED_BODY()

public:
	UPlan();

	virtual void PostInitProperties() override;

	UFUNCTION(BlueprintCallable, Category = "Plan")
	void PushPlan(TArray<TSubclassOf<UStage>> InPlan);

protected:

	void CreateCurrentStageInstance(TSubclassOf<UStage> InStageTemplate);

	UFUNCTION()
	void Advance();

	UFUNCTION()
	void QueueAdvance();

	UFUNCTION()
	void AbortStage(const EStageAbortReason OnStageAborted);

	bool CanAdvance() const;
	
	UPROPERTY()
	UVillagerManager* CachedVillagerManager;

	FTimerHandle PlanTickHandle;

	TFunction<void()> AdvancePtr;

	TArray<TSubclassOf<UStage>> CurrentPlan;
	TArray<TArray<TSubclassOf<UStage>>> Plans;

	UPROPERTY()
	UStage* CurrentStageInstance;

	bool IsPlanActive;

private:

	//Plans that should be run all the time, or should happen ambiently.  This is essentially housekeeping, like "find a new job", or "find somewhere to live".
	TArray<TSubclassOf<UStage>> RecursiveStages;

	void ClearCurrentStage();
};
