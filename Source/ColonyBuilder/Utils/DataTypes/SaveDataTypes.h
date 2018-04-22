// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SaveDataTypes.generated.h"

USTRUCT()
struct FSaveData
{
	GENERATED_USTRUCT_BODY()
};

USTRUCT()
struct FBuildingSaveData : public FSaveData
{
	GENERATED_USTRUCT_BODY()

	UClass* BuildingClass;
	FTransform BuildingTransform;
};

UCLASS()
class COLONYBUILDER_API USaveDataTypes : public UObject
{
	GENERATED_BODY()
};
