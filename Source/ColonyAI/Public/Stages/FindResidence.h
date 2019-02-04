// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "Plan/Stage.h"
#include "FindResidence.generated.h"

//////////////////////////////////////////////////////////////////////////
// Find this villager a place to live
//////////////////////////////////////////////////////////////////////////

UCLASS()
class COLONYAI_API UFindResidence : public UStage
{
	GENERATED_BODY()

public:
	virtual void OnStart() override;
};
