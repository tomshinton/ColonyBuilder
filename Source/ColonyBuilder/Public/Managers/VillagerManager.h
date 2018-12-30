// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "ColonyManager.h"
#include "Utils/DataTypes/SaveDataTypes.h"
#include "BaseVillager.h"
#include "VillagerManager.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class COLONYBUILDER_API UVillagerManager : public UColonyManager
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = Spawning)
	void CreateVillagerFromSavedata(FVillagerSaveData& Savedata);

	UFUNCTION(BlueprintCallable, Category = Spawning)
	void CreateVillagerAtLocation(const FVector Location);

	void RegisterNewVillager(ABaseVillager* InNewVillager);

	void SetBaseVillagerClass(TSubclassOf<ABaseVillager> InBaseVillagerClass) { BaseVillagerClass = InBaseVillagerClass; }
	
private:

	TSubclassOf<ABaseVillager> BaseVillagerClass;
	TArray<TWeakObjectPtr<ABaseVillager>> SpawnedVillagers;
};
