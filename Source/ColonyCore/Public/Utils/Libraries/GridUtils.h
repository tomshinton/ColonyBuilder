#pragma once

#include "Kismet/GameplayStatics.h"
#include "Utils/DataTypes/BuildingDataTypes.h"
#include "CollisionChannels.h"
#include "BuildSystem/Buildables/PointValidationRules.h"
#include "Core/Player/RTSPlayerController.h"

class UColonyBuilderGameModeBase;
class UBuildingData;

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

	static FORCEINLINE FHitResult GetFloorInfoLocation(UWorld* WorldContext, float InX, float InY)
	{
		FHitResult HitRes;
		FCollisionQueryParams TraceParams;

		GetControllerRef(WorldContext)->GetReferenceActor()->ActorLineTraceSingle(HitRes, FVector(InX, InY, 1000), FVector(InX, InY, -1000), ECC_Camera, TraceParams);
		if (HitRes.Actor.Get())
		{
			return HitRes;
		}

		return FHitResult();		
	}

	static FORCEINLINE TArray<EInvalidReason> IsPointValid(AActor* WorldContext, FSubBuilding& InPoint, UBuildingData* BuildingData)
	{
		TArray<EInvalidReason> Reasons;
		UPointValidationRules* PointRules = BuildingData->PointRules;

		//no valid points?
		if (!PointRules)
		{
			Reasons.Add(EInvalidReason::NoPointRules);
			return Reasons;
		}

		//is overlapping
		const FName TraceTag("Point Overlap Trace");

		if (/*OverlapsAtPoint.Num() > 0 &&*/ !Reasons.Contains(EInvalidReason::LegalOverlap))
		{
			Reasons.AddUnique(EInvalidReason::IllegalOverlap);
		}

		//too low, too high?
		if (InPoint.Location.Z < PointRules->MinPointHeight) 
		{
			Reasons.AddUnique(EInvalidReason::PointIsTooLow);
		}

		if (InPoint.Location.Z > PointRules->MaxPointHeight)
		{
			Reasons.AddUnique(EInvalidReason::PointIsTooHigh);
		}

		if (InPoint.PointType == EPointType::GridPoint)
		{
			//X Coord Bounds
			if(InPoint.MaxCoord.X > PointRules->MaxX-1 || InPoint.MaxCoord.Y > PointRules->MaxY-1)
			{
				Reasons.AddUnique(EInvalidReason::GridIsTooBig);
			}
			//Y Coord Bounds
			if(InPoint.MaxCoord.X < PointRules->MinX -1 || InPoint.MaxCoord.Y < PointRules->MinY -1)
			{
				Reasons.AddUnique(EInvalidReason::GridIsTooSmall);
			}
		}
		
		//is too steep
		if (FMath::Abs(InPoint.LocationNormal.Rotation().Pitch - 90.f) > PointRules->MaxPitch)
		{
			Reasons.AddUnique(EInvalidReason::SurfaceTooSteep);
		}

		//The only problem with the point is it is overlapping with something of the same type - this is fine
		if (Reasons.Num() == 1 && Reasons.Contains(EInvalidReason::LegalOverlap))
		{
			InPoint.IsValidPoint = false;
			return Reasons;
		}
		else if (Reasons.Num() >= 1) //must be more than one reason
		{
			InPoint.IsValidPoint = false;
		}
		else
		{
			InPoint.IsValidPoint = true;
		}

		return Reasons;
	}
};