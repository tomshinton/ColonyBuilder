// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "MoveTo.h"
#include "MoveTo_Work.generated.h"

//////////////////////////////////////////////////////////////////////////
// Encapsulated stage to move this pawn to their Work, assuming they have on - Aborts Plan if unsuccessful
//////////////////////////////////////////////////////////////////////////

UCLASS()
class COLONYCORE_API UMoveTo_Work : public UMoveTo
{
	GENERATED_BODY()

public:
	UMoveTo_Work();

protected:
	virtual void OnSetMoveToLocation() override;
};
