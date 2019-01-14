// ColonyBuilder Project, personal project by Tom Shinton

#include "MoveTo.h"
#include "Utils/Libraries/MathDefs.h"
#include "BaseVillager.h"
#include <AI/Navigation/NavigationTypes.h>
#include <AI/Navigation/NavigationData.h>
#include <AI/Navigation/NavFilters/NavigationQueryFilter.h>
#include <AI/Navigation/NavigationSystem.h>
#include <DrawDebugHelpers.h>
#include <Components/SplineComponent.h>

DEFINE_LOG_CATEGORY_STATIC(MoveToLog, Log, All);

UMoveTo::UMoveTo()
	: Super()
	, ShouldPath(false)
	, CurrentDistance(0.f)
	, MaxDistance(0.f)
	, NavProperties(ABaseVillager::AgentRadius, ABaseVillager::AgentHeight)
{
	TickInterval = SIXTY_FPS_AS_FLOAT;
	IsTickEnabled = true;

	NavProperties.bCanWalk = true;
	NavProperties.bCanFly = false;
}

void UMoveTo::OnStart()
{
	GetRoute();
}

void UMoveTo::GetRoute()
{
	ShouldPath = false;

	FPathFindingQuery NavParams;
	NavParams.StartLocation = GetVillagerOuter()->GetActorLocation();
	NavParams.EndLocation = GetWorld()->GetNavigationSystem()->GetRandomPointInNavigableRadius(GetWorld(), NavParams.StartLocation, 20000);

	ANavigationData* NavData = GetWorld()->GetNavigationSystem()->MainNavData;
	NavParams.QueryFilter = UNavigationQueryFilter::GetQueryFilter<UNavigationQueryFilter>(*NavData);

	FNavPathQueryDelegate PathFoundDelegate;
	PathFoundDelegate.BindUObject(this, &UMoveTo::OnRouteFound);
	
	GetWorld()->GetNavigationSystem()->FindPathAsync(NavProperties, NavParams, PathFoundDelegate, EPathFindingMode::Regular);
}

void UMoveTo::OnRouteFound(uint32 PathId, ENavigationQueryResult::Type ResultType, FNavPathSharedPtr NavPointer)
{
	if (NavPointer->GetPathPoints().Num() <= 0)
	{
		UE_LOG(MoveToLog, Error, TEXT("Could not find any valid NavPoints for MoveTo"));
		GetRoute();
		return;
	}

	const float DistanceFromCurrentLocation = FVector::Dist(NavPointer->GetPathPoints()[0], GetVillagerOuter()->GetActorLocation());

	if (DistanceFromCurrentLocation > 100 /*Nav agent radius - refactor*/)
	{
		UE_LOG(MoveToLog, Error, TEXT("Movement delta was bigger than expected when starting route - did we teleport? Distance: %f"), DistanceFromCurrentLocation);
		GetRoute();
		return;
	}

	if (PathSpline)
	{
		PathSpline->ClearSplinePoints(true);
	}
	else
	{
		PathSpline = NewObject<USplineComponent>(this, FName("MoveTo Spline"));
		PathSpline->ClearSplinePoints(true);
	}



	for (const FNavPathPoint& Point : NavPointer->GetPathPoints())
	{
		PathSpline->AddSplinePoint(Point.Location, ESplineCoordinateSpace::World, true);
	}

	MaxDistance = PathSpline->GetSplineLength();
	
	FNavPathPoint lastNavPoint;
	bool isFirstPoint = true;
	for (FNavPathPoint navPoint : NavPointer->GetPathPoints())
	{
		DrawDebugPoint(GetWorld(), navPoint.Location, 10, FColorList::CadetBlue, false, 100);
		if (!isFirstPoint)
		{
			DrawDebugLine(GetWorld(), lastNavPoint.Location, navPoint.Location, FColorList::HunterGreen, false, 100, -1, 10.f);
		}
		lastNavPoint = navPoint;
		isFirstPoint = false;
	}

	CurrentDistance = 0;
	ShouldPath = true;
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

		BaseVillager->SetActorLocation(LocationAtDistance);
		const FRotator NewRotation = FRotator(BaseVillager->GetActorRotation().Pitch, YawAtDistance, BaseVillager->GetActorRotation().Roll);
		BaseVillager->SetActorRotation(NewRotation);
	}

	if (FMath::IsNearlyEqual(CurrentDistance, MaxDistance) || CurrentDistance >= MaxDistance)
	{
		FinishExecute();
	}
}