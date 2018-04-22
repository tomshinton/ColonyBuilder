// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "Interface.h"
#include "Utils/DataTypes/SaveDataTypes.h"
#include "SavableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType, MinimalAPI)
class USavableInterface : public UInterface
{
	GENERATED_BODY()
};

class COLONYBUILDER_API ISavableInterface
{
	GENERATED_BODY()

public:
	virtual FSaveData GetSaveData();
	virtual void ReceiveSaveData();
};
