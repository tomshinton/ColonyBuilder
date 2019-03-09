// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "AI/Journal/Keys.h"
#include "Stage.generated.h"

enum class EStageAbortReason : uint8;
class ABaseVillager;

//////////////////////////////////////////////////////////////////////////
// Base stage class for any action that could take place within a Plan.  Base class for both blueprintable ones, and async ones, as well as parallel
//////////////////////////////////////////////////////////////////////////

UCLASS(BlueprintType, Blueprintable)
class COLONYCORE_API UStage : public UObject
{
	GENERATED_BODY()

public:

	UStage();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage Flow")
	void StageTick(const float DeltaTime);

	//Virtual implementation to account for cpp defined stages 
	virtual void OnStageTick(const float DeltaTime);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStageCompleted);
	UPROPERTY(BlueprintAssignable, Category = Callbacks)
	FOnStageCompleted OnStageCompleted;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStageAborted, const EStageAbortReason, AbortReason);
	UPROPERTY(BlueprintAssignable, Category = Callbacks)
	FOnStageAborted OnStageAborted;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage Flow")
	void Start();

	//Virtual implementation to account for cpp defined stages
	virtual void OnStart();

	UFUNCTION(BlueprintCallable, Category = "Stage Flow")
	void FinishExecute();

	UFUNCTION(BlueprintCallable, Category = "Stage Flow")
	void AbortExecute(const EStageAbortReason InAbortReason);

	UFUNCTION(BlueprintCallable, Category = "Debug")
	void StageCallback(const FString String, const float Duration, const bool PrintToScreen);

	UFUNCTION(BlueprintPure, Category = "World")
	virtual class UWorld* GetWorld() const override;

	UFUNCTION(BlueprintPure, Category = "Stage Flow")
	bool GetIsActive() const { return IsActive; }
	
	virtual void PostInitProperties() override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Tick")
	float TickInterval;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Tick")
	bool IsTickEnabled;

protected:

	ABaseVillager* GetVillagerOuter() const;

private:

	bool IsActive;
	
	FTimerHandle TickStageHandle;

	TArray<FJournalKey> BoundKeys;
};
