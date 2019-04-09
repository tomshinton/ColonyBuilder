// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "BuildAction.h"
#include "BuildActionFF.generated.h"

//////////////////////////////////////////////////////////////////////////
// Build Action to drive placement of single buildings, like houses or workplaces
//////////////////////////////////////////////////////////////////////////

DECLARE_LOG_CATEGORY_EXTERN(BuildActionFFLog, Log, All);

UCLASS()
class COLONYCORE_API UBuildActionFF : public UBuildAction
{
	GENERATED_BODY()

public:

	virtual void OnBuild() override;

	virtual void StartAction() override;
	virtual void CompleteAction() override;
	virtual void CancelAction() override;
	virtual void SpawnGhost() override;

	UFUNCTION()
	void UpdateGhost(const FVector& InRoundedLocUnderMouse);

	void RotatePlacement();

private:

	bool IsBuilding;
};
