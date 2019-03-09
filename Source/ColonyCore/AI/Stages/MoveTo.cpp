// ColonyBuilder Project, personal project by Tom Shinton

#include "MoveTo.h"
#include "Utils/Libraries/MathDefs.h"

#include <AI/Navigation/NavigationTypes.h>
#include <AI/Navigation/NavigationData.h>
#include <AI/Navigation/NavFilters/NavigationQueryFilter.h>
#include <AI/Navigation/NavigationSystem.h>
#include <Components/SplineComponent.h>

#include "Utils/Libraries/CollisionChannels.h"
#include "Utils/Libraries/ManagerUtils.h"
#include "AI/Pawns/BaseVillager.h"

DEFINE_LOG_CATEGORY(MoveToLog)

UMoveTo::UMoveTo()
	: Super()
	, ShouldPath(false)
	, CurrentDistance(0.f)
	, MaxDistance(0.f)
	, NavProperties(ABaseVillager::AgentRadius, ABaseVillager::AgentHeight)
	, AdhereFrequency(100.f)
	, ShouldGarrisonOnEnd(false)
{
	TickInterval = SIXTY_FPS_AS_FLOAT;
	IsTickEnabled = true;

	NavProperties.bCanWalk = true;
	NavProperties.bCanFly = false;
}

void UMoveTo::OnStart()
{
	UE_LOG(MoveToLog, Log, TEXT("Starting MoveTo stage"));

	SetMoveToLocation();

	if (FMath::IsNearlyZero(FVector::Dist2D(MoveToLocation, GetVillagerOuter()->GetActorLocation())))
	{
		UE_LOG(MoveToLog, Warning, TEXT("Already at target location"))

		FinishExecute();		
		return;
	}

	ProjectLocationToNavMesh();

	OnTraceCompleteDelegate.BindLambda([this] (const FTraceHandle& InHandle, FTraceDatum& InData)
	{
		if (InData.OutHits.Num() > 0)
		{
			FHitResult& CurrHit = InData.OutHits[0];
			AdheredPoints.Add(CurrHit.Location);

			if (AdheredPoints.Num() == ExpectedAdheredPoints)
			{
				OnAdhereComplete();
			}
		}
	});

	GetRoute();
}

void UMoveTo::SetTargetBuildingID_Implementation(){}

void UMoveTo::OnSetMoveToLocation(){}

void UMoveTo::SetMoveToLocation_Implementation()
{
	OnSetMoveToLocation();
}

void UMoveTo::GetRoute()
{
	UE_LOG(MoveToLog, Log, TEXT("GetRoute started"));

	ShouldPath = false;

	FPathFindingQuery NavParams;
	NavParams.StartLocation = GetVillagerOuter()->GetActorLocation();
	NavParams.EndLocation = MoveToLocation;

	ANavigationData* NavData = GetWorld()->GetNavigationSystem()->MainNavData;
	NavParams.QueryFilter = UNavigationQueryFilter::GetQueryFilter<UNavigationQueryFilter>(*NavData);

	FNavPathQueryDelegate PathFoundDelegate;
	PathFoundDelegate.BindUObject(this, &UMoveTo::OnRouteFound);
	
	GetWorld()->GetNavigationSystem()->FindPathAsync(NavProperties, NavParams, PathFoundDelegate, EPathFindingMode::Regular);
}

void UMoveTo::OnRouteFound(uint32 PathId, ENavigationQueryResult::Type ResultType, FNavPathSharedPtr NavPointer)
{
	UE_LOG(MoveToLog, Log, TEXT("Route found!"));

	if (NavPointer->GetPathPoints().Num() <= 0)
	{
		UE_LOG(MoveToLog, Error, TEXT("Could not find any valid NavPoints for MoveTo"));
		GetRoute();
		return;
	}

	TArray<FNavPathPoint> PathPoints = NavPointer->GetPathPoints();

	if (PathPoints.Num() > 0)
	{
		const FVector OwnerLocation = GetVillagerOuter()->GetActorLocation();
		const float DistanceFromCurrentLocation = FVector::Dist(PathPoints[0], GetVillagerOuter()->GetActorLocation());

		if (DistanceFromCurrentLocation > ABaseVillager::AgentRadius * 2)
		{
			UE_LOG(MoveToLog, Log, TEXT("Movement delta was particularly large (%f), adding a compliment point to spline"), DistanceFromCurrentLocation);
			PathPoints.Insert(FNavPathPoint(OwnerLocation), 0);
		}

		if (PathSpline)
		{
			PathSpline->ClearSplinePoints(true);
		}
		else
		{
			PathSpline = NewObject<USplineComponent>(this, FName("MoveTo Spline"));
			PathSpline->RegisterComponent();
			PathSpline->ClearSplinePoints(true);
		}

		//Build Guide Spline
		for (int32 PointIndex = 0; PointIndex < NavPointer->GetPathPoints().Num(); PointIndex++)
		{
			const FNavPathPoint& CurrPoint = NavPointer->GetPathPoints()[PointIndex];
			PathSpline->AddSplinePoint(CurrPoint.Location, ESplineCoordinateSpace::World, true);
			PathSpline->SetSplinePointType(PointIndex, ESplinePointType::Linear, true);
		}

		MaxDistance = PathSpline->GetSplineLength();

		if (FMath::IsNearlyZero(MaxDistance))
		{
			UE_LOG(MoveToLog, Error, TEXT("Distance of 0 detected on spline - aborting"));
			AbortExecute(EStageAbortReason::Failure);
			return;
		}

		//Make sure we have at least 1 point to adhere, else we'll get stuck
		if (MaxDistance < AdhereFrequency)
		{
			AdhereFrequency = MaxDistance;
		}

		//Build Adherant Spline
		ExpectedAdheredPoints = MaxDistance / AdhereFrequency;

		UE_LOG(MoveToLog, Log, TEXT("Starting adherance to landscape - Distance of %f adhered over %i points"), MaxDistance, ExpectedAdheredPoints);

		TArray<FVector2D> NewPoints;
		for (int32 NewPoint = 0; NewPoint <= ExpectedAdheredPoints; NewPoint++)
		{
			FVector LocAtPoint = PathSpline->GetLocationAtDistanceAlongSpline(FMath::Clamp<float>(NewPoint*AdhereFrequency, 0, MaxDistance), ESplineCoordinateSpace::World);
			NewPoints.Add(FVector2D(LocAtPoint.X, LocAtPoint.Y));
			AdherePoint(LocAtPoint);
		}
	}
	else
	{
		UE_LOG(MoveToLog, Error, TEXT("Could not build path"));
		AbortExecute(EStageAbortReason::Failure);
	}
}

void UMoveTo::AdherePoint(FVector& InPointToAdhere)
{
	if (UWorld* World = GetWorld())
	{
		const float TraceOffset = 5000.f;
		const FVector StartPoint = InPointToAdhere + FVector(0.f, 0.f, TraceOffset);
		const FVector EndPoint = InPointToAdhere - FVector(0.f, 0.f, TraceOffset);
		FHitResult HitRes;

		FCollisionQueryParams QueryParams;
		QueryParams.bTraceAsyncScene = true;

		FCollisionResponseParams ResponseParams;

		World->AsyncLineTraceByChannel(EAsyncTraceType::Single, StartPoint, EndPoint, CC_GROUND, QueryParams, ResponseParams, &OnTraceCompleteDelegate);
	}
}

void UMoveTo::OnAdhereComplete()
{
	//Build actual spline
	PathSpline->ClearSplinePoints(true);
	for (int32 i = 0; i < AdheredPoints.Num(); i++)
	{
		PathSpline->AddSplinePoint(AdheredPoints[i], ESplineCoordinateSpace::World, true);
		PathSpline->SetSplinePointType(i, ESplinePointType::Linear);
	}

	UngarrisonPawn();

	CurrentDistance = 0;
	ShouldPath = true;
}

void UMoveTo::ProjectLocationToNavMesh()
{
	if (UWorld* World = GetWorld())
	{
		FNavLocation ProjectedLocation;
		GetWorld()->GetNavigationSystem()->ProjectPointToNavigation(MoveToLocation, ProjectedLocation);

		MoveToLocation = ProjectedLocation.Location;
	}
}

void UMoveTo::OnStageTick(const float DeltaTime)
{
	if (ShouldPath)
	{
		ABaseVillager* BaseVillager = GetVillagerOuter();
		const float VillagerMoveSpeed = BaseVillager->MoveSpeed;
		CurrentDistance += DeltaTime * VillagerMoveSpeed;

		const FVector LocationAtDistance = PathSpline->GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);
		const float YawAtDistance = PathSpline->GetWorldRotationAtDistanceAlongSpline(CurrentDistance).Yaw;

		BaseVillager->SetActorLocation(LocationAtDistance + FVector(0.f, 0.f, ABaseVillager::AgentHeight));
		const FRotator NewRotation = FRotator(BaseVillager->GetActorRotation().Pitch, YawAtDistance, BaseVillager->GetActorRotation().Roll);
		BaseVillager->SetActorRotation(NewRotation);

		if (CanFinishMove())
		{
			FinishMove();
		}
	}
}

bool UMoveTo::CanFinishMove() const
{
	return FMath::IsNearlyEqual(CurrentDistance, MaxDistance) || CurrentDistance >= MaxDistance;
}

void UMoveTo::FinishMove()
{
	if (ShouldGarrisonOnEnd)
	{
		GarrisonPawn();
	}

	PathSpline->DestroyComponent();
	PathSpline->UnregisterComponent();
	FinishExecute();
}

void UMoveTo::GarrisonPawn()
{
	if (UConstructionManager* ConstructionManager = GetManager<UConstructionManager>(GetVillagerOuter()->GetWorld()))
	{
		ConstructionManager->GarrisonPawn(GetVillagerOuter(), TargetBuildingID);
	}
}

void UMoveTo::UngarrisonPawn()
{
	if (UConstructionManager* ConstructionManager = GetManager<UConstructionManager>(GetVillagerOuter()->GetWorld()))
	{
		ConstructionManager->UngarrisonPawn(GetVillagerOuter(), TargetBuildingID);
	}
}
