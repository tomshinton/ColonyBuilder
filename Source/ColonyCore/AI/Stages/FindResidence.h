// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "AI/Plan/Stage.h"
#include "FindResidence.generated.h"

//////////////////////////////////////////////////////////////////////////
// Find this villager a place to live
//////////////////////////////////////////////////////////////////////////

UCLASS()
class COLONYCORE_API UFindResidence : public UStage
{
	GENERATED_BODY()

public:
	virtual void OnStart() override;
};
