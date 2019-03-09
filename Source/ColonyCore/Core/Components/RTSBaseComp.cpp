// ColonyBuilder Project, personal project by Tom Shinton

#include "RTSBaseComp.h"

#include "Utils/Libraries/DebugUtils.h"
#include "../Player/PlayerPawn.h"
#include "../Player/RTSPlayerController.h"

URTSBaseComp::URTSBaseComp()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URTSBaseComp::BeginPlay()
{
	Super::BeginPlay();

	OwningPawn = Cast<APlayerPawn>(GetOwner());

	if (OwningPawn)
	{
		OwningController = Cast<ARTSPlayerController>(OwningPawn->GetController());
	}
}
