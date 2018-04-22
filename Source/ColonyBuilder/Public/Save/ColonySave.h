// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Utils/DataTypes/SaveDataTypes.h"
#include "ColonySave.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(SaveSlot, Log, All);

UCLASS()
class COLONYBUILDER_API UColonySave : public USaveGame
{
	GENERATED_BODY()

	UColonySave();

public:
	FString SlotName;
	FDateTime SaveTime;
	TArray<FSaveData> SavedActors;
};
