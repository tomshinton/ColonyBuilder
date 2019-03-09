// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "BuildActionLinear.generated.h"

//////////////////////////////////////////////////////////////////////////
// Build Action to drive linear placement of buildings, like walls and paths
//////////////////////////////////////////////////////////////////////////

UCLASS()
class COLONYCORE_API UBuildActionLinear : public UBuildAction
{
	GENERATED_BODY()
	
public:
	virtual void StartAction() override;
	virtual void CompleteAction() override;
	virtual void CancelAction() override;

	virtual void SpawnGhost() override;
};
