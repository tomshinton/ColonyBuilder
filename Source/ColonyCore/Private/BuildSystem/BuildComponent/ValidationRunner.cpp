// ColonyBuilder Project, personal project by Tom Shinton

#include "ValidationRunner.h"
#include "ColonyBuilderGameModeBase.h"
#include <DrawDebugHelpers.h>
#include <WorldCollision.h>
#include "CollisionChannels.h"
#include "PointValidationRules.h"

#pragma once

DEFINE_LOG_CATEGORY(ValidationRunnerLog)

FValidationRunner::FValidationRunner(AGhost* InValidatingGhost)
	: ValidatingGhost(InValidatingGhost)
	, TracesComplete(0)
	, CellVisuals(nullptr)
{
	UE_LOG(ValidationRunnerLog, Log, TEXT("Spinning up ValidationRunner"));
}

FValidationRunner::~FValidationRunner()
{
	UE_LOG(ValidationRunnerLog, Log, TEXT("Spinning down up ValidationRunner"));
}

void FValidationRunner::FootprintMatch(UWorld& InWorld, const TArray<FSubBuilding>& InTestPoints)
{
	FlushPersistentDebugLines(&InWorld);

	if (UColonyGridSettings* GridSettings = GetMutableDefault<UColonyGridSettings>())
	{
		const int32 GridSize = GridSettings->GridSize;

		TestedPoints.Empty();
		TracesComplete = 0;
		const float BoxExtent = GridSize * 0.5;

		FOverlapDelegate OnTraceCompleteDelegate;
		OnTraceCompleteDelegate.BindLambda([&, this, BoxExtent](const FTraceHandle& InHandle, FOverlapDatum& InData)
		{
			TracesComplete++;

			if (InData.OutOverlaps.Num() == 1)
			{
#if ValidVisuals
				DrawDebugLine(&InWorld, InData.Pos + FVector(0.f, 0.f, 500.f), InData.Pos + FVector(0.f, 0.f, -100.f), FColor::Green, true, -1.f, 0, 5.f);
#endif //ValidVisuals
				TestedPoints.Add(FSubBuilding(FVector(InData.Pos.X, InData.Pos.Y, 0.f), EPointType::BuildingPoint));
			}
			else
			{
#if ValidVisuals
				DrawDebugLine(&InWorld, InData.Pos + FVector(0.f, 0.f, 500.f), InData.Pos + FVector(0.f, 0.f, -100.f), FColor::Red, true, -1.f, 0, 1.f);
#endif //ValidVisuals
			}

			if (TracesComplete >= InTestPoints.Num())
			{
				OnFootprintMatchComplete.ExecuteIfBound(TestedPoints);
				AlignToGround(InWorld, TestedPoints);
			}
			else
			{
#if ValidVisuals
				GEngine->AddOnScreenDebugMessage(1, 1, FColor::White, FString::Printf(TEXT("Trace %i of %i"), TracesComplete, InTestPoints.Num() - 1));
#endif //ValidVisuals
			}
		});

		FCollisionQueryParams Params;
		Params.bTraceAsyncScene = true;
		Params.bTraceComplex = true;

		for (const FSubBuilding& TestPoint : InTestPoints)
		{
			InWorld.AsyncOverlapByChannel(TestPoint.Location, FQuat(), CC_GHOST, FCollisionShape::MakeBox(FVector(BoxExtent, BoxExtent, 500.f)), Params, FCollisionResponseParams::DefaultResponseParam, &OnTraceCompleteDelegate);
		}
	}
}

void FValidationRunner::Validate(UWorld& InWorld, const TSubclassOf<AGhost>& InGhostClass, const TArray<FSubBuilding>& InTestPoints, const UBuildingData& InBuildingData)
{
	OnGroundAlignComplete.BindLambda([&, this](const TArray<FSubBuilding>& InTestPoints)
	{
		//Calls Validation
		ApplyPointRules(InWorld, InTestPoints, InBuildingData);
	});

	if (InGhostClass == AGhostFF::StaticClass())
	{
		OnFootprintMatchComplete.BindLambda([&, this](const TArray<FSubBuilding>& InTestPoints)
		{
			AlignToGround(InWorld, InTestPoints);
		});

		FootprintMatch(InWorld, InTestPoints);
	}
	else
	{
		AlignToGround(InWorld, InTestPoints);
	}	
}

void FValidationRunner::AlignToGround(UWorld& InWorld, const TArray<FSubBuilding>& InTestPoints)
{
	TracesComplete = 0;

	FCollisionQueryParams QueryParams;
	QueryParams.bTraceAsyncScene = true;

	FCollisionResponseParams ResponseParams;

	for (const FSubBuilding& TestPoint : InTestPoints)
	{
		FTraceDelegate OnTraceCompleteDelegate;
		const FVector TraceStart = TestPoint.Location - FVector(0.f, 0.f, 1000.f);
		const FVector TraceEnd = TestPoint.Location + FVector(0.f, 0.f, 5000.f);

#if ValidVisuals
		DrawDebugLine(&InWorld, TraceStart, TraceEnd, FColor::Green, true, 1.f, 0, 5.f);
#endif //ValidVisuals

		OnTraceCompleteDelegate.BindLambda([&, this](const FTraceHandle& InHandle, FTraceDatum& InData)
		{
			TracesComplete++;

			if (InData.OutHits.Num() > 0)
			{
#if ValidVisuals
				DrawDebugPoint(&InWorld, InData.OutHits[0].Location, 10.f, FColor::Green, false);
#endif //ValidVisuals

				FSubBuilding NewBuilding;
				NewBuilding.SetHitResult(InData.OutHits[0]);
				TestedPoints.Add(NewBuilding);
			}

			if (TestedPoints.Num() == InTestPoints.Num())
			{
				OnGroundAlignComplete.ExecuteIfBound(TestedPoints);
			}
		});

		InWorld.AsyncLineTraceByChannel(EAsyncTraceType::Single, TraceStart, TraceEnd, CC_GROUND, QueryParams, ResponseParams, &OnTraceCompleteDelegate);
	}
}

void FValidationRunner::ApplyPointRules(UWorld& InWorld, const TArray<FSubBuilding>& InTestPoints, const UBuildingData& InBuildingData)
{
	TArray<FSubBuilding> ValidatedPoints;

	UPointValidationRules* PointRules = InBuildingData.PointRules;

	for (const FSubBuilding& TestPoint : InTestPoints)
	{
		if (UColonyGridSettings* GridSettings = GetMutableDefault<UColonyGridSettings>())
		{
			const int32 GridSize = GridSettings->GridSize;

			FSubBuilding ValidatedPoint = TestPoint;

			//Do we have valid points?
			if (!PointRules)
			{
				ValidatedPoint.AddFailureReason(EInvalidReason::NoPointRules);
				break;
			}

			FTraceDelegate OnPointOverlapComplete;
			FCollisionResponseParams ResponseParams;

			OnPointOverlapComplete.BindLambda([&, this](const FTraceHandle& InHandle, FTraceDatum& InData)
			{
				IsLegalOverlap(ValidatedPoint, InData, InBuildingData);
			});

			const FVector TraceStart = ValidatedPoint.Location + FVector(0.f, 0.f, 1000.f);
			const FVector TraceEnd = ValidatedPoint.Location + FVector(0.f, 0.f, -1000.f);
			const FCollisionShape TestBox = FCollisionShape::MakeBox(FVector(GridSize * 0.5f, GridSize * 0.5f, 1.f));
			InWorld.AsyncSweepByChannel(EAsyncTraceType::Multi, TraceStart, TraceEnd, CC_PLACEMENT, TestBox, FCollisionQueryParams(), FCollisionResponseParams(), &OnPointOverlapComplete);

			ValidatedPoints.Add(ValidatedPoint);
		}
	}

	//ShouldFailWholePlacement(ValidatedPoints, InBuildingData);

	OnValidationComplete.ExecuteIfBound(false, ValidatedPoints);
	CreateCellVisuals(InWorld, ValidatedPoints);

}

void FValidationRunner::IsLegalOverlap(FSubBuilding& OutTestPoint, const FTraceDatum& InTraceData, const UBuildingData& InBuildingData)
{
	const UPointValidationRules& PointRules = *InBuildingData.PointRules;

	if (InTraceData.OutHits.Num() > 0)
	{
		for (const FHitResult& HitRes : InTraceData.OutHits)
		{
			const AActor& OverlappedActor = *HitRes.Actor.Get();
			const UClass* OverlappedClass = OverlappedActor.GetClass();

			for (UClass* Class : PointRules.AllowedOverlapClasses)
			{
				if (OverlappedClass->IsChildOf(Class))
				{
					continue;
				}
				else if(PointRules.AllowSameTypeOverlaps)
				{
					if (OverlappedClass == InBuildingData.BodyClass || OverlappedClass == InBuildingData.BuildingClass)
					{
						OutTestPoint.AddFailureReason(EInvalidReason::LegalOverlap);
					}
				}
			}
		}
	}
}

void FValidationRunner::CreateCellVisuals(UWorld& InWorld, const TArray<FSubBuilding>& InPointToVisualise)
{
	if(CellVisuals && !CellVisuals->IsPendingKill())
	{
		//CellVisuals->Destroy();
	}

	if (!CellVisuals && ValidatingGhost)
	{
		CellVisuals = InWorld.SpawnActor<ACellVisuals>(ValidatingGhost->GetActorLocation(), ValidatingGhost->GetActorRotation(), FActorSpawnParameters());
		if (CellVisuals)
		{
			CellVisuals->Visualise(InPointToVisualise);
		}
	}
}

