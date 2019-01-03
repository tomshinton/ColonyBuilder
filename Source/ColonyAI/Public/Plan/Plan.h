// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Plan.generated.h"

class UStage;

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

	UFUNCTION()
	void Advance();

protected:

	void CreateCurrentStageInstance(TSubclassOf<UStage> InStageTemplate);

private:

	UFUNCTION()
	void TickPlan(const float DeltaTime);

	bool CanAdvance() const;

	FTimerHandle PlanTickHandle;
	FOnPlanTick OnTick;

	TArray<TSubclassOf<UStage>> CurrentPlan;
	TArray<TArray<TSubclassOf<UStage>>> Plans;

	UPROPERTY()
	UStage* CurrentStageInstance;

	bool IsPlanActive;
};
