#pragma once

#include "ColonyManagers.generated.h"

class UColonyManager;

//////////////////////////////////////////////////////////////////////////
// Base UObject for ColonyManagers - accessed via GetUnmutableDefaults
//////////////////////////////////////////////////////////////////////////

UCLASS(config = Game, defaultconfig)
class COLONYCORE_API UColonyManagers : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, config, Category = "Managers")
	TArray<TSubclassOf<UColonyManager>> Managers;
};
