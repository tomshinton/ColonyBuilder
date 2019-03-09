// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "Utils/DataTypes/SaveDataTypes.h"
#include <GameFramework/SaveGame.h>
#include "ColonySave.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(SaveSlot, Log, All);

UCLASS()
class COLONYCORE_API UColonySave : public USaveGame
{
	GENERATED_BODY()

	UColonySave();

public:
	UFUNCTION(BlueprintCallable, Category = "Save Game")
	void Heartbeat();

public:

	UPROPERTY(SaveGame)
	FString SlotName;

	UPROPERTY(SaveGame)
	FDateTime SaveTime;

	UPROPERTY(SaveGame)
	TArray<FBuildingSaveData> SavedBuildables;

	UPROPERTY(SaveGame)
	FPlayerSaveData PlayerSaveData;

	UPROPERTY(SaveGame)
	TArray<FVillagerSaveData> SavedVillagers;
};
