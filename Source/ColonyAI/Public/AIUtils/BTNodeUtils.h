#pragma once
#include <EnvironmentQuery/EnvQueryManager.h>
#include <EnvironmentQuery/EnvQueryTypes.h>
#include <Kismet/KismetMathLibrary.h>
#include <AI/Navigation/NavigationSystem.h>
#include <AI/Navigation/NavigationData.h>

static FVector GetRandomNavigablePointInVolume(const FVector VolumeLocation, const FVector VolumeBounds, const UObject* WorldContext, const int32 PointsToGenerate = 20)
{
	TArray<FVector> RandomPoints;

	for (int32 i = 0; i < PointsToGenerate; i++)
	{
		const FVector& NewPoint = UKismetMathLibrary::RandomPointInBoundingBox(VolumeLocation, VolumeBounds);

		FNavLocation NavLocation;
		UNavigationSystem* NavSys = WorldContext->GetWorld()->GetNavigationSystem();

		if (NavSys->ProjectPointToNavigation(NewPoint, NavLocation))
		{
			RandomPoints.AddUnique(NavLocation.Location);
		}
	}

	if (RandomPoints.Num() > 0)
	{
		const int32 RandomIndex = FMath::CeilToInt(FMath::RandRange(0, RandomPoints.Num() - 1));
		const FVector RandomPoint = RandomPoints[RandomIndex];

		return RandomPoint;
	}

	return FVector();
}