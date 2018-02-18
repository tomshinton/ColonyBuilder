// ColonyBuilder Project, personal project by Tom Shinton

#include "BuildComponent.h"

#include "BuildingData.h"

#include "RTSPlayerController.h"
#include "PlayerPawn.h"
#include "Ghost.h"

#include "Kismet/GameplayStatics.h"
#include "ColonyBuilderGameModeBase.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UBuildComponent::UBuildComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SpawnedGhost = nullptr;
}

void UBuildComponent::BeginPlay()
{
	ControllerRef = Cast<ARTSPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void UBuildComponent::SetEnabled(bool InEnabled)
{
	Super::SetEnabled(InEnabled);

	if (IsEnabled)
	{

		if (ControllerRef && !ControllerRef->OnMouseMoved.IsBound())
		{
			ControllerRef->OnMouseMoved.AddDynamic(this, &UBuildComponent::UpdateMouseCoords);
		}

		OwningPawn->OnStartConfirmAction.BindUObject(this, &UBuildComponent::StartPlacement);
		OwningPawn->OnEndConfirmAction.BindUObject(this, &UBuildComponent::EndPlacement);
		OwningPawn->OnCancelAction.BindUObject(this, &UBuildComponent::CancelBuild);
		OwningPawn->OnRotatePlacement.BindUObject(this, &UBuildComponent::RotatePlacement);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(BuildIntermediatePosTimer);

		if (ControllerRef && ControllerRef->OnMouseMoved.IsBound()) //General cleanup, dont want this running if its not needed
		{
			ControllerRef->OnMouseMoved.RemoveDynamic(this, &UBuildComponent::UpdateMouseCoords);

			if (SpawnedGhost)
			{
				SpawnedGhost->Destroy();
				SpawnedGhost = nullptr;
			}
		}

		OwningPawn->OnStartConfirmAction.Unbind();
		OwningPawn->OnEndConfirmAction.Unbind();
		OwningPawn->OnCancelAction.Unbind();
		OwningPawn->OnRotatePlacement.Unbind();
	}
}

void UBuildComponent::UpdateMouseCoords(FVector InCurrMouseCoords, FVector InRoundedMouseCoords)
{
	CurrMouseCoords = InCurrMouseCoords;
	CurrRoundedMouseCoords = InRoundedMouseCoords;

	UpdateGhostLocation();
}

void UBuildComponent::RotatePlacement()
{
	if (SpawnedGhost)
	{
		FRotator NewRotation = SpawnedGhost->GetActorRotation();
		NewRotation.Yaw += RotationRate;
		SpawnedGhost->SetActorRotation(NewRotation);
	}
}

void UBuildComponent::StartBuildingFromClass(UBuildingData* BuildingData)
{
	SetEnabled(true);
	CurrentBuildingData = BuildingData;

	if (CurrentBuildingData)
	{
		if (SpawnedGhost)
		{
			SpawnedGhost->Destroy();
		}
		FActorSpawnParameters SpawnParams;
		SpawnedGhost = GetWorld()->SpawnActor<AGhost>(CurrRoundedMouseCoords, FRotator::ZeroRotator, SpawnParams);
		SpawnedGhost->SetGhostInfo(CurrentBuildingData);
	}
	else
	{
		CancelBuild();
	}
}

void UBuildComponent::StartPlacement()
{
	MouseLocationAtBuildingStart = CurrRoundedMouseCoords;
	GetWorld()->GetTimerManager().SetTimer(BuildIntermediatePosTimer, this, &UBuildComponent::BuildIntermediatePositions, 0.5f, true);
}

void UBuildComponent::BuildIntermediatePositions()
{
	switch (CurrentBuildingData->ConstructionMethod)
	{
	case EConstructionMethod::FireAndForget:
		break;
	case EConstructionMethod::Grid:
		GeneratedPositions = BuildGridPoints();
		break;
	case EConstructionMethod::Linear:
		GeneratedPositions = BuildLinearPoints();
		break;
	}
}

TArray<FIntermediateBuildingLocation> UBuildComponent::AlignPositionsToGround(TArray<FIntermediateBuildingLocation> Positions)
{
	TArray<FIntermediateBuildingLocation> NewPoints;
	for (FIntermediateBuildingLocation Pos : Positions)
	{
		FHitResult HitRes;

		FCollisionQueryParams TraceParams;
		TraceParams.bTraceComplex = true;

		TraceParams.AddIgnoredActor(GetOwner());
		FVector TraceStart = Pos.Location + FVector(0, 0, 1000);
		FVector TraceEnd = Pos.Location - FVector(0, 0, 1000);
		if (ControllerRef->GetReferenceActor())
		{
			ControllerRef->GetReferenceActor()->ActorLineTraceSingle(HitRes, TraceStart, TraceEnd, ECC_Camera, TraceParams);
			if (HitRes.Actor.Get())
			{
				FIntermediateBuildingLocation NewPoint = Pos;
				NewPoint.Location = HitRes.Location;

				NewPoints.AddUnique(NewPoint);
			}
		}
	}

	return NewPoints;
}

TArray<FIntermediateBuildingLocation> UBuildComponent::BuildLinearPoints()
{
	TArray<FIntermediateBuildingLocation> OutPoints;

	return OutPoints;
}

TArray<FIntermediateBuildingLocation> UBuildComponent::BuildGridPoints()
{
	TArray<FIntermediateBuildingLocation> OutPoints;

	float XDelta = CurrRoundedMouseCoords.X - MouseLocationAtBuildingStart.X;
	float YDelta = CurrRoundedMouseCoords.Y - MouseLocationAtBuildingStart.Y;

	int32 XDeltaAsUnits = XDelta / AColonyBuilderGameModeBase::GridSize;
	int32 YDeltaAsUnits = YDelta / AColonyBuilderGameModeBase::GridSize;

	int8 XDir;
	int8 YDir;

	if (XDeltaAsUnits > 0)
	{
		XDir = 1;
	}
	else
	{
		XDir = -1;
	}

	if (YDeltaAsUnits > 0)
	{
		YDir = 1;
	}
	else
	{
		YDir = -1;
	}

	for(int32 x = 0; x <= FMath::Abs(XDeltaAsUnits); x++)
	{
		for (int32 y = 0; y <= FMath::Abs(YDeltaAsUnits); y++)
		{
			FVector NewPoint = MouseLocationAtBuildingStart;

			NewPoint.X  =  NewPoint.X + (AColonyBuilderGameModeBase::GridSize * (x*XDir));
			NewPoint.Y = NewPoint.Y + (AColonyBuilderGameModeBase::GridSize * (y*YDir));

			FIntermediateBuildingLocation NewLocation;
			NewLocation.Location = NewPoint;

			OutPoints.AddUnique(NewLocation);
		}
	}

	OutPoints = AlignPositionsToGround(OutPoints);
	/*DEBUG*/
	FlushPersistentDebugLines(GetWorld());
	FString PointsCallback = FString::FromInt(OutPoints.Num()) + " total points";
	for (FIntermediateBuildingLocation Loc : OutPoints)
	{
		DrawDebugSphere(GetWorld(), MouseLocationAtBuildingStart, AColonyBuilderGameModeBase::GridSize / 2, 8, FColor::Red, false, 0.5f);
		DrawDebugSphere(GetWorld(), Loc.Location, AColonyBuilderGameModeBase::GridSize / 2, 8, FColor::Emerald, false, 0.5f);
	}

	return OutPoints;
}

void UBuildComponent::EndPlacement()
{
	TArray<FIntermediateBuildingLocation> LocalPositions = GeneratedPositions;
	GetWorld()->GetTimerManager().ClearTimer(BuildIntermediatePosTimer);

	FVector CurrentGhostLoc;
	FRotator CurrentGhostRot;

	if (SpawnedGhost && SpawnedGhost->GetIsValid())
	{
		CurrentGhostLoc = SpawnedGhost->GetActorLocation();
		CurrentGhostRot = SpawnedGhost->GetActorRotation();
		SpawnedGhost->Destroy();
		FActorSpawnParameters SpawnParams;
		ABuildableBase* NewBuilding = GetWorld()->SpawnActor<ABuildableBase>(CurrentBuildingData->BuildingClass, CurrentGhostLoc, CurrentGhostRot);
		NewBuilding->IntermediateBuildingLocations = GeneratedPositions;

		SetEnabled(false);
	}
}

void UBuildComponent::CancelBuild()
{	
	SetEnabled(false);
}

void UBuildComponent::UpdateGhostLocation()
{
	if (SpawnedGhost)
	{
		SpawnedGhost->SetActorLocation(CurrRoundedMouseCoords);
	}
}

