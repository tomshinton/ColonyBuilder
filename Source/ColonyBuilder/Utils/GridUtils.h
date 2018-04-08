#pragma once

#include "CoreMinimal.h"

#include "RTSPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "PointValidationRules.h"
#include "DebugUtils.h"

#include "DataTypes/BuildingDataTypes.h"
#include "ColonyBuilderGameModeBase.h"

#include "GridUtils.generated.h"

UENUM(BlueprintType)
enum class EInvalidReason : uint8
{
	NoPointRules	UMETA(DisplayName = "No Display Rules"),
	PointIsTooHigh	UMETA(DisplayName = "Point it too high"),
	PointIsTooLow	UMETA(DisplayName = "Point is too low"),
	GridIsTooBig	UMETA(DisplayName = "Grid is too big"),
	GridIsTooSmall	UMETA(DisplayName = "Grid is too small"),
	SurfaceTooSteep	UMETA(DisplayName = "Surface is too steep"),
	NoWorldContext	UMETA(DisplayName = "No World Context"),
	IllegalOverlap	UMETA(DisplayName = "Illegal Overlap at Point")
};

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

	static FORCEINLINE TArray<EInvalidReason> IsPointValid(AActor* WorldContext, FSubBuilding& InPoint, UPointValidationRules* PointRules)
	{
		TArray<EInvalidReason> Reasons;

		//no valid points?
		if (!PointRules)
		{
			Reasons.Add(EInvalidReason::NoPointRules);
		}

		//is overlapping
		const FName TraceTag("Point Overlap Trace");

		TArray<FHitResult> OverlapsAtPoint;
		FCollisionQueryParams TraceParams(FName(TEXT("BoxTrace at point")));
		TraceParams.TraceTag = TraceTag;

		const FVector& StartPoint = InPoint.Location + FVector(0, 0, 1000);
		const FVector& EndPoint = InPoint.Location + FVector(0, 0, -100);
		FCollisionShape Box = FCollisionShape::MakeBox(FVector(AColonyBuilderGameModeBase::GridSize / 2.1, AColonyBuilderGameModeBase::GridSize / 2.1, 1.f));
		WorldContext->GetWorld()->SweepMultiByChannel(OverlapsAtPoint, StartPoint, EndPoint, FRotator::ZeroRotator.Quaternion(), ECC_Visibility, Box, TraceParams);

		for (int32 i = OverlapsAtPoint.Num() - 1; i >= 0; i--)
		{
			AActor* OverlappedActor = OverlapsAtPoint[i].Actor.Get();
				
			for (UClass* Class : PointRules->AllowedOverlapClasses)
			{
				if ((OverlappedActor->GetClass())->IsChildOf(Class))
				{
					OverlapsAtPoint.RemoveAt(i);
					continue;
				}
			}
		}

		if (OverlapsAtPoint.Num() > 0)
		{
			Reasons.Add(EInvalidReason::IllegalOverlap);
		}

		//too low, too high?
		if (InPoint.Location.Z < PointRules->MinPointHeight) 
		{
			Reasons.Add(EInvalidReason::PointIsTooLow);
		}

		if (InPoint.Location.Z > PointRules->MaxPointHeight)
		{
			Reasons.Add(EInvalidReason::PointIsTooHigh);
		}

		if (InPoint.PointType == EPointType::GridPoint)
		{
			//X Coord Bounds
			//if (InPoint.MaxCoord.X > (InPoint.MaxCoord.X - 1) || InPoint.MaxCoord.Y > (InPoint.MaxCoord.Y - 1))
			if(InPoint.MaxCoord.X > PointRules->MaxX-1 || InPoint.MaxCoord.Y > PointRules->MaxY-1)
			{
				Reasons.Add(EInvalidReason::GridIsTooBig);
			}
			//Y Coord Bounds
			//if (InPoint.MaxCoord.X < (InPoint.MaxCoord.X - 1) || InPoint.MaxCoord.Y < (InPoint.MaxCoord.Y - 1))
			if(InPoint.MaxCoord.X < PointRules->MinX -1 || InPoint.MaxCoord.Y < PointRules->MinY -1)
			{
				Reasons.Add(EInvalidReason::GridIsTooSmall);
			}
		}
		
		//is too steep
		if (FMath::Abs(InPoint.LocationNormal.Rotation().Pitch - 90.f) > PointRules->MaxPitch)
		{
			Reasons.Add(EInvalidReason::SurfaceTooSteep);
		}

		if (Reasons.Num() > 0)
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