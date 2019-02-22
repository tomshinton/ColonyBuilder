// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "ColonyManager.h"
#include "Utils/DataTypes/SaveDataTypes.h"
#include "BaseVillager.h"
#include <Function.h>
#include "VillagerManager.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class COLONYBUILDER_API UVillagerManager : public UColonyManager
{
	GENERATED_BODY()

public:

	UVillagerManager();

	virtual void Init(const TFunction<void()> InitCallback) override;

	UFUNCTION(BlueprintCallable, Category = Spawning)
	void CreateVillagerFromSavedata(FVillagerSaveData& Savedata);

	UFUNCTION(BlueprintCallable, Category = Spawning)
	void CreateVillagerAtLocation(const FVector Location);

	void RegisterNewVillager(ABaseVillager* InNewVillager);

	void SetBaseVillagerClass(TSubclassOf<ABaseVillager> InBaseVillagerClass) { BaseVillagerClass = InBaseVillagerClass; }

	void PushAdvance(TFunction<void()> InFunc, const bool IsCritical = false);
	
	int32 GetAdvanceQueueNum() const { return AdvanceFuncArray.Num(); };

protected:

	void TickPlanAdvance();

private:

	TArray<TFunction<void()>> AdvanceFuncArray;

	FTimerHandle TickPlanHandle;

	TSubclassOf<ABaseVillager> BaseVillagerClass;
	TArray<TWeakObjectPtr<ABaseVillager>> SpawnedVillagers;
};
