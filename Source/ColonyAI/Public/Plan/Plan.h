// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Plan.generated.h"

class UStage;
class UVillagerManager;

//////////////////////////////////////////////////////////////////////////
// Base Plan for AI FiniteStateMachine
//////////////////////////////////////////////////////////////////////////

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlanTick, const float, DeltaTime);

DECLARE_STATS_GROUP(TEXT("PlanAI"), STATGROUP_PlanAI, STATCAT_Advanced);

UCLASS()
class COLONYAI_API UPlan : public UObject
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

	bool CanAdvance() const;

	void QueueAdvance();

	UPROPERTY()
	UVillagerManager* CachedVillagerManager;

	TFunction<void()> AdvancePtr;

	FTimerHandle PlanTickHandle;

	TArray<TSubclassOf<UStage>> CurrentPlan;
	TArray<TArray<TSubclassOf<UStage>>> Plans;

	UPROPERTY()
	UStage* CurrentStageInstance;

	bool IsPlanActive;
};
