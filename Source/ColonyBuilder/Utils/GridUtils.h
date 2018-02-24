#pragma once

#include "CoreMinimal.h"

#include "RTSPlayerController.h"
#include "Kismet/GameplayStatics.h"

#include "GridUtils.generated.h"

UCLASS()
class UGridUtils : public UObject 
{

	GENERATED_BODY()
public:

	static ARTSPlayerController* GetControllerRef(UWorld* WorldContext)
	{
		return Cast<ARTSPlayerController>(UGameplayStatics::GetPlayerController(WorldContext, 0));
	}

	static FORCEINLINE float GetFloorZAtLocation(UWorld* WorldContext, float InX, float InY)
	{
		FHitResult HitRes;
		FCollisionQueryParams TraceParams;

		GetControllerRef(WorldContext)->GetReferenceActor()->ActorLineTraceSingle(HitRes, FVector(InX, InY, 1000), FVector(InX, InY, -1000), ECC_Camera, TraceParams);

		if (HitRes.Actor.Get())
		{
			return HitRes.Location.Z;
		}

		return 0;
	}

};