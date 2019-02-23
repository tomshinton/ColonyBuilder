// ColonyBuilder Project, personal project by Tom Shinton

#include "RTSBaseComp.h"
#include "PlayerPawn.h"
#include "RTSPlayerController.h"

#include "Utils/Libraries/DebugUtils.h"

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
