// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "Interface.h"

#include "ConstructionInterface.generated.h"

class UBuildingData;

UINTERFACE(BlueprintType, MinimalAPI)
class UConstructionInterface : public UInterface
{
	GENERATED_BODY()
};

class COLONYBUILDER_API IConstructionInterface
{
	GENERATED_BODY()

public:
	virtual void StartConstruction(UBuildingData* InBuildingData) = 0;
	virtual void UpdateConstructionTime(float InUpdatedTime) = 0;
	virtual void FinishConstruction() = 0;
};
