// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "Stages/MoveTo.h"
#include "MoveTo_Home.generated.h"

//////////////////////////////////////////////////////////////////////////
// Encapsulated stage to move this pawn to their Home, assuming they have on - Aborts Plan if unsuccessful
//////////////////////////////////////////////////////////////////////////

UCLASS()
class COLONYAI_API UMoveTo_Home : public UMoveTo
{
	GENERATED_BODY()

	UMoveTo_Home();

protected:
	virtual void OnSetMoveToLocation() override;
};
