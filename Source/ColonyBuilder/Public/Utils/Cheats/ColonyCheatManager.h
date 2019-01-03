// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "ColonyCheatManager.generated.h"

//////////////////////////////////////////////////////////////////////////
// Colony Game cheat class
//////////////////////////////////////////////////////////////////////////

UCLASS()
class COLONYBUILDER_API UColonyCheatManager : public UCheatManager
{
	GENERATED_BODY()

	UFUNCTION(Exec)
	void ToggleAIReadout();

private:

	FTimerHandle AIReadoutTimerHandle;
	void ShowAIReadout() const;
	bool AIReadoutActive;
};
