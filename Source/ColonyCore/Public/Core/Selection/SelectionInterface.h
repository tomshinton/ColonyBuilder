// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "Interface.h"

#include "SelectionInterface.generated.h"

UINTERFACE(BlueprintType, MinimalAPI)
class USelectionInterface : public UInterface
{
	GENERATED_BODY()
};

class COLONYCORE_API ISelectionInterface
{
	GENERATED_BODY()
public:
	virtual void OnReceiveHover() = 0;
	virtual void OnEndHover() = 0;

	virtual void OnSelect() = 0;
	virtual void OnEndSelect() = 0;

};
