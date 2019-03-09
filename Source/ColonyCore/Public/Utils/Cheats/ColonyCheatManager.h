// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include <GameFramework/CheatManager.h>

#include "ColonyCheatManager.generated.h"

//////////////////////////////////////////////////////////////////////////
// Colony Game cheat class
//////////////////////////////////////////////////////////////////////////

UCLASS()
class COLONYCORE_API UColonyCheatManager : public UCheatManager
{
	GENERATED_BODY()

	UFUNCTION(Exec)
	void ToggleAIReadout(const bool ShowReadout);

	UFUNCTION(Exec)
	void BuildActions(const bool ShowReadout);
};
