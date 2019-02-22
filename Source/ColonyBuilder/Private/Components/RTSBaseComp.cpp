// ColonyBuilder Project, personal project by Tom Shinton

#include "RTSBaseComp.h"
#include "PlayerPawn.h"
#include "RTSPlayerController.h"

#include "Utils/Libraries/DebugUtils.h"

// Sets default values for this component's properties
URTSBaseComp::URTSBaseComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	OwningPawn = Cast<APlayerPawn>(GetOwner());

	if (OwningPawn)
	{
		OwningController = Cast<ARTSPlayerController>(OwningPawn->GetController());
	}
}