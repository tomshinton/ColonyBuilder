// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

class USceneComponent;

#include "BuildSystem/Buildables/Ghost/GhostBase.h"
#include "GhostFF.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(GhostFFLog, Error, All);

UCLASS()
class COLONYCORE_API AGhostFF : public AGhost
{
	GENERATED_BODY()

	AGhostFF();

public:
	virtual void GhostStart() override;
	virtual void BuildTestPoints() override;

private:

	UPROPERTY()
	TArray<USceneComponent*> DuplicatedObjects;

	FVector GhostBounds;
	FVector GhostOrigin;

	int32 ExpectedTestCells;
};
