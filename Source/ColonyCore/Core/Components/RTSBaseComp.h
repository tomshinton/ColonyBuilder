// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "RTSBaseComp.generated.h"

class APlayerPawn;
class ARTSPlayerController;

DECLARE_LOG_CATEGORY_EXTERN(BaseComponentLog, Log, All);

UCLASS()
class COLONYCORE_API URTSBaseComp : public UActorComponent
{
	GENERATED_BODY()

public:	

	URTSBaseComp();

	virtual void BeginPlay() override;
	
protected:

	UPROPERTY()
	APlayerPawn* OwningPawn;

	UPROPERTY()
	ARTSPlayerController* OwningController;
};
