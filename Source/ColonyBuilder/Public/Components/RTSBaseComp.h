// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "RTSBaseComp.generated.h"

class APlayerPawn;
class ARTSPlayerController;

DECLARE_LOG_CATEGORY_EXTERN(BaseComponentLog, Log, All);

UCLASS()
class COLONYBUILDER_API URTSBaseComp : public UActorComponent
{
	GENERATED_BODY()

public:	

	URTSBaseComp();
	
protected:

	UPROPERTY()
	APlayerPawn* OwningPawn;

	UPROPERTY()
	ARTSPlayerController* OwningController;
};
