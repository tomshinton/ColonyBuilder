#pragma once

#include "Professions.generated.h"

UCLASS(abstract)
class COLONYCORE_API UProfessionBase : public UObject
{
	GENERATED_BODY()

public:
	FString ReadableName;

};

UCLASS()
class COLONYCORE_API UBuilder : public UProfessionBase
{
	GENERATED_BODY()
};