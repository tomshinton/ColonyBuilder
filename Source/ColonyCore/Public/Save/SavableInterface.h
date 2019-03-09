// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "SavableInterface.generated.h"

struct FBuildingSaveData;

UINTERFACE(BlueprintType, MinimalAPI)
class USavableInterface : public UInterface
{
	GENERATED_BODY()
};

class COLONYCORE_API ISavableInterface
{
	GENERATED_BODY()

public:
	virtual FBuildingSaveData GetBuildingSaveData() = 0;
	virtual void LoadBuildingSaveData(FBuildingSaveData LoadedData) = 0;
};
