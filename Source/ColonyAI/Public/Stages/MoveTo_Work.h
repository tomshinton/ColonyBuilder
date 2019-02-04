// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "Stages/MoveTo.h"
#include "MoveTo_Work.generated.h"

//////////////////////////////////////////////////////////////////////////
// Encapsulated stage to move this pawn to their Work, assuming they have on - Aborts Plan if unsuccessful
//////////////////////////////////////////////////////////////////////////

UCLASS()
class COLONYAI_API UMoveTo_Work : public UMoveTo
{
	GENERATED_BODY()

public:
	UMoveTo_Work();

protected:
	virtual void OnSetMoveToLocation() override;
};
