// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "BuildActionGrid.generated.h"

//////////////////////////////////////////////////////////////////////////
// Build Action to drive placement of grid based buildings, like farms
//////////////////////////////////////////////////////////////////////////

UCLASS()
class COLONYCORE_API UBuildActionGrid : public UBuildAction
{
	GENERATED_BODY()

public:
	virtual void StartAction() override;
	virtual void CompleteAction() override;
	virtual void CancelAction() override;

	virtual void SpawnGhost() override;
};
