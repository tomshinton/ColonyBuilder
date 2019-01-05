// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "Stage.generated.h"

//////////////////////////////////////////////////////////////////////////
// Base stage class for any action that could take place within a Plan.  Base class for both blueprintable ones, and async ones, as well as parallel
//////////////////////////////////////////////////////////////////////////

UCLASS(BlueprintType, Blueprintable)
class COLONYAI_API UStage : public UObject
{
	GENERATED_BODY()

	UStage();

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage Flow")
	void OnStageTick(const float DeltaTime);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStageCompleted);
	UPROPERTY(BlueprintAssignable, Category = Callbacks)
	FOnStageCompleted OnStageCompleted;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage Flow")
	void Start();

	UFUNCTION(BlueprintCallable, Category = "Stage Flow")
	void FinishExecute();

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

private:

	bool IsActive;
	
	FTimerHandle TickStageHandle;
};
