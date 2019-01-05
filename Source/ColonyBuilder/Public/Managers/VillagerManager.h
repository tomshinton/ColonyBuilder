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

	virtual void PostInitProperties() override;

	UFUNCTION(BlueprintCallable, Category = Spawning)
	void CreateVillagerFromSavedata(FVillagerSaveData& Savedata);

	UFUNCTION(BlueprintCallable, Category = Spawning)
	void CreateVillagerAtLocation(const FVector Location);

	void RegisterNewVillager(ABaseVillager* InNewVillager);

	void SetBaseVillagerClass(TSubclassOf<ABaseVillager> InBaseVillagerClass) { BaseVillagerClass = InBaseVillagerClass; }

	void PushAdvance(TFunction<void()> InFunc);
	
protected:

	void TickPlanAdvance();

private:

	TArray<TFunction<void()>> AdvanceFuncArray;

	FTimerHandle TickPlanHandle;

	TSubclassOf<ABaseVillager> BaseVillagerClass;
	TArray<TWeakObjectPtr<ABaseVillager>> SpawnedVillagers;
};
