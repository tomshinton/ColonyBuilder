#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

#if WITH_EDITOR
#include "UnrealEd.h"
#endif //WITH_EDITOR

DECLARE_LOG_CATEGORY_EXTERN(ColonyAILog, All, All)

class FColonyAIModule : public IModuleInterface
{

public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};