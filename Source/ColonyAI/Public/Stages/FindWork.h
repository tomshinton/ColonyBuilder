// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "Plan/Stage.h"
#include "FindWork.generated.h"

//////////////////////////////////////////////////////////////////////////
// Find this villager a place to work
//////////////////////////////////////////////////////////////////////////

UCLASS()
class COLONYAI_API UFindWork : public UStage
{
	GENERATED_BODY()

public:
	virtual void OnStart() override;
};
