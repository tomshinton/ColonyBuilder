#pragma once
#include "NoExportTypes.h"

#include "Professions.generated.h"

UCLASS(abstract)
class UProfessionBase : public UObject
{
	GENERATED_BODY()

public:
	FString ReadableName;

};

UCLASS()
class UBuilder : public UProfessionBase
{
	GENERATED_BODY()
};