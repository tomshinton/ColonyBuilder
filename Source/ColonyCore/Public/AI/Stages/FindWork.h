// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

class UStage;

#include "FindWork.generated.h"

//////////////////////////////////////////////////////////////////////////
// Find this villager a place to work
//////////////////////////////////////////////////////////////////////////

UCLASS()
class COLONYCORE_API UFindWork : public UStage
{
	GENERATED_BODY()

public:
	virtual void OnStart() override;
};
