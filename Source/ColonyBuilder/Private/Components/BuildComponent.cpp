// ColonyBuilder Project, personal project by Tom Shinton

#include "BuildComponent.h"

#include "BuildingData.h"

#include "RTSPlayerController.h"
#include "PlayerPawn.h"
#include "Ghost.h"

#include "Kismet/GameplayStatics.h"
#include "Utils/Libraries/GridUtils.h"

#include "ColonyBuilderGameModeBase.h"
#include "BuildableBase.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY(BuildCompLogError);

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
		SubBuildings.Empty();
		HasStartedBuilding = false;

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

	UpdateGhost();
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

void UBuildComponent::StartBuildingFromClass(UBuildingData* InBuildingData)
{
	SetEnabled(true);
	BuildingData = InBuildingData;

	if (BuildingData)
	{
		if (SpawnedGhost)
		{
			SpawnedGhost->Destroy();
		}
		FActorSpawnParameters SpawnParams;
		SpawnedGhost = GetWorld()->SpawnActor<AGhost>(CurrRoundedMouseCoords, FRotator::ZeroRotator, SpawnParams);
		SpawnedGhost->SetGhostInfo(BuildingData);
	}
	else
	{
		CancelBuild();
	}
}

void UBuildComponent::StartPlacement(bool IsNewPlacement)
{
	if (SpawnedGhost && SpawnedGhost->GetIsValid())
	{
		if (IsNewPlacement)
		{
			MouseLocationAtBuildingStart = CurrRoundedMouseCoords;
		}

		SpawnedGhost->SetBaseGhostVisibility(BuildingData->ShouldHideBaseMeshOnStartPlacement);

		GetWorld()->GetTimerManager().SetTimer(BuildIntermediatePosTimer, this, &UBuildComponent::BuildIntermediatePositions, 0.1f, true);
		HasStartedBuilding = true;
	}
}

void UBuildComponent::BuildIntermediatePositions()
{
	switch (BuildingData->ConstructionMethod)
	{
	case EConstructionMethod::FireAndForget:
		break;
	case EConstructionMethod::Grid:
		SubBuildings = BuildGridPoints();
		break;
	case EConstructionMethod::Linear:
		SubBuildings = BuildLinearPoints();
		ValidatePointTypesToUnique();
		break;
	}

	AlignAndOrientate();

}

TArray<FSubBuilding> UBuildComponent::BuildLinearPoints()
{
	TArray<FSubBuilding> OutPoints;

	float XDelta = CurrRoundedMouseCoords.X - MouseLocationAtBuildingStart.X;
	float YDelta = CurrRoundedMouseCoords.Y - MouseLocationAtBuildingStart.Y;

	int32 XDeltaAsUnits = XDelta / AColonyBuilderGameModeBase::GridSize;
	int32 YDeltaAsUnits = YDelta / AColonyBuilderGameModeBase::GridSize;

	int8 XDir;
	int8 YDir;

#pragma region Directions
	if (XDeltaAsUnits >= 0)
	{
		XDir = 1;
	}
	else
	{
		XDir = -1;
	}

	if (YDeltaAsUnits >= 0)
	{
		YDir = 1;
	}
	else
	{
		YDir = -1;
	}
#pragma endregion Directions

	FVector NewPoint = MouseLocationAtBuildingStart;

	if (FMath::Abs(XDeltaAsUnits) > FMath::Abs(YDeltaAsUnits))
	{
		//Generate the points along the X
		for (int32 x = 0; x <= FMath::Abs(XDeltaAsUnits); x++)
		{
			NewPoint.X = MouseLocationAtBuildingStart.X + (AColonyBuilderGameModeBase::GridSize * (x*XDir));
			FSubBuilding NewLocation(NewPoint, FVector2D(XDir, 0), EPointType::LinearPoint, ESubBuildingType::LinearBody);

			OutPoints.AddUnique(NewLocation);
		}
	}
	else
	{
		//Generate the points along the Y
		for (int32 y = 0; y <= FMath::Abs(YDeltaAsUnits); y++)
		{
			NewPoint.Y = MouseLocationAtBuildingStart.Y + (AColonyBuilderGameModeBase::GridSize * (y*YDir));

			FSubBuilding NewLocation(NewPoint, FVector2D(0, YDir), EPointType::LinearPoint, ESubBuildingType::LinearBody);
			OutPoints.AddUnique(NewLocation);
		}
	}

	return OutPoints;
}

TArray<FSubBuilding> UBuildComponent::BuildGridPoints()
{
	TArray<FSubBuilding> OutPoints;

	float XDelta = CurrRoundedMouseCoords.X - MouseLocationAtBuildingStart.X;
	float YDelta = CurrRoundedMouseCoords.Y - MouseLocationAtBuildingStart.Y;

	int32 XDeltaAsUnits = XDelta / AColonyBuilderGameModeBase::GridSize;
	int32 YDeltaAsUnits = YDelta / AColonyBuilderGameModeBase::GridSize;

	int8 XDir;
	int8 YDir;

#pragma region Directions
	if (XDeltaAsUnits >= 0)
	{
		XDir = 1;
	}
	else
	{
		XDir = -1;
	}

	if (YDeltaAsUnits >= 0)
	{
		YDir = 1;
	}
	else
	{
		YDir = -1;
	}
#pragma endregion Directions

	for(int32 x = 0; x <= FMath::Abs(XDeltaAsUnits); x++)
	{
		for (int32 y = 0; y <= FMath::Abs(YDeltaAsUnits); y++)
		{
			FVector NewPoint = MouseLocationAtBuildingStart;

			NewPoint.X  =  NewPoint.X + (AColonyBuilderGameModeBase::GridSize * (x*XDir));
			NewPoint.Y = NewPoint.Y + (AColonyBuilderGameModeBase::GridSize * (y*YDir));

			FSubBuilding NewLocation(NewPoint, EPointType::GridPoint, ESubBuildingType::Body, FVector2D(FMath::Abs(x), FMath::Abs(y)), FVector2D(FMath::Abs(XDeltaAsUnits), FMath::Abs(YDeltaAsUnits)));
			OutPoints.AddUnique(NewLocation);
		}
	}

	return OutPoints;
}

TArray<FSubBuilding> UBuildComponent::BuildFFPoints()
{
	TArray<FSubBuilding> OutPoints;
	const FVector& CachedBounds = SpawnedGhost->GetCachedGhostBounds();
	float Higherbounds;
	int32 BoundsToUnits;

	CachedBounds.X > CachedBounds.Y ? Higherbounds = CachedBounds.X : Higherbounds = CachedBounds.Y;
	BoundsToUnits = FMath::Abs(FMath::CeilToInt(FMath::Abs(Higherbounds) / AColonyBuilderGameModeBase::GridSize) + 5);

	for (int32 x = -BoundsToUnits/2; x <= BoundsToUnits/2; x++)
	{
		for (int32 y = -BoundsToUnits / 2; y <= BoundsToUnits / 2; y++)
		{
			FVector NewPoint = SpawnedGhost->GetActorLocation();

			NewPoint.X = NewPoint.X + (AColonyBuilderGameModeBase::GridSize * x);
			NewPoint.Y = NewPoint.Y + (AColonyBuilderGameModeBase::GridSize * y);

			FSubBuilding NewLocation(NewPoint, EPointType::BuildingPoint);
			OutPoints.AddUnique(NewLocation);
		}
	}

	SpawnedGhost->SetActorEnableCollision(true);

	for (int32 i = OutPoints.Num() - 1; i >= 0; i--)
	{
		bool HitGhost = false;
		const FSubBuilding& CurrPoint = OutPoints[i];

		//is overlapping
		const FName TraceTag("Point Overlap Trace");

		TArray<FHitResult> OverlapsAtPoint;
		FCollisionQueryParams TraceParams(FName(TEXT("BoxTrace at point")));
		TraceParams.TraceTag = TraceTag;

		const FVector& StartPoint = CurrPoint.Location + FVector(0, 0, 1000);
		const FVector& EndPoint = CurrPoint.Location + FVector(0, 0, -100);
		FCollisionShape Box = FCollisionShape::MakeBox(FVector(AColonyBuilderGameModeBase::GridSize / 2.1, AColonyBuilderGameModeBase::GridSize / 2.1, 1.f));
		GetWorld()->SweepMultiByChannel(OverlapsAtPoint, StartPoint, EndPoint, FRotator::ZeroRotator.Quaternion(), ECC_Visibility, Box, TraceParams);

		for (int32 i = OverlapsAtPoint.Num() - 1; i >= 0; i--)
		{
			AActor* OverlappedActor = OverlapsAtPoint[i].Actor.Get();
			if (Cast<AGhost>(OverlappedActor))
			{
				HitGhost = true;
				break;
			}
		}

		if (!HitGhost)
		{
			OutPoints.RemoveAt(i);
		}
	}

	SpawnedGhost->SetActorEnableCollision(false);
	return OutPoints;
}

void UBuildComponent::AlignAndOrientate()
{
	for (FSubBuilding& Point : SubBuildings)
	{
		//Align point to ground
		const FHitResult LocalPointInfo = UGridUtils::GetFloorInfoLocation(GetWorld(), Point.Location.X, Point.Location.Y);
		Point.Location.Z = LocalPointInfo.Location.Z;
		Point.LocationNormal = LocalPointInfo.ImpactNormal;

		if (Point.SubBuildingType == ESubBuildingType::Edge)
		{
			if (Point.CurrCoord.X == 0 || Point.CurrCoord.X == Point.MaxCoord.X)
			{
				Point.Direction = FVector2D(0, 1);
			}

			if (Point.CurrCoord.Y == 0 || Point.CurrCoord.Y == Point.MaxCoord.Y)
			{
				Point.Direction = FVector2D(1, 0);
			}
		}
	}
}

void UBuildComponent::EndPlacement()
{
	if (HasStartedBuilding)
	{
		GetWorld()->GetTimerManager().ClearTimer(BuildIntermediatePosTimer);

		FVector CurrentGhostLoc;
		FRotator CurrentGhostRot;

		if (SpawnedGhost && SpawnedGhost->GetIsValid())
		{
			CurrentGhostLoc = SpawnedGhost->GetActorLocation();
			CurrentGhostRot = SpawnedGhost->GetActorRotation();

			SubBuildings = SpawnedGhost->SubBuildings;

			SpawnedGhost->Destroy();
			FActorSpawnParameters SpawnParams;

			//Does this building type actually spawn a building at the end?
			if (BuildingData->BuildingClass && BuildingData->ConstructionMethod == EConstructionMethod::FireAndForget)
			{
				ABuildableBase* NewBuilding = GetWorld()->SpawnActor<ABuildableBase>(BuildingData->BuildingClass, CurrentGhostLoc, CurrentGhostRot);
				NewBuilding->SubBuildings = SubBuildings;
			}

			for (FSubBuilding& SubBuilding : SubBuildings)
			{
				if (!SubBuilding.IsValidPoint)
				{
					continue;
				}

				//Spawn plants and whatnot
				if (SubBuilding.SubBuildingType == ESubBuildingType::Body)
				{
					AGridBodyBase* NewSubBuilding = GetWorld()->SpawnActor<AGridBodyBase>(BuildingData->BodyClass, SubBuilding.Location, FRotator(0, 0, 0));
				}

				if (SubBuilding.PointType == EPointType::LinearPoint)
				{
					ABuildableBase* NewSubBuilding = GetWorld()->SpawnActor<ABuildableBase>(BuildingData->BuildingClass, SubBuilding.Location, FVector(SubBuilding.Direction.X, SubBuilding.Direction.Y, 0).Rotation());
					NewSubBuilding->MeshComponent->SetStaticMesh(*BuildingData->SubBuildingMeshes.Find(SubBuilding.SubBuildingType));
				}
			}
		}

		SetEnabled(false);
		SubBuildings.Empty();
	}
}

void UBuildComponent::CancelBuild()
{	
	SetEnabled(false);
}

void UBuildComponent::UpdateGhost()
{
	if (SpawnedGhost)
	{

		if (BuildingData)
		{
			if(BuildingData->ConstructionMethod == EConstructionMethod::FireAndForget)
			{
				SubBuildings = BuildFFPoints();
			}
			else if(!HasStartedBuilding)
			{
				SubBuildings.Empty();
				FSubBuilding DefaultSubBuilding(CurrRoundedMouseCoords, EPointType::BuildingPoint);
				SubBuildings.AddUnique(DefaultSubBuilding);
			}

			AlignAndOrientate();
		}

		SpawnedGhost->UpdateGhost(CurrRoundedMouseCoords, SubBuildings);
	}
}

void UBuildComponent::ValidatePointTypesToUnique()
{
	for (int32 i = 0; i <= SubBuildings.Num() - 1; i++)
	{
		if (i == 0 || i == SubBuildings.Num() - 1)
		{
			SubBuildings[i].SubBuildingType = ESubBuildingType::LinearTerminator;
			continue;
		}

		if (i == 1 )
		{
			SubBuildings[i].SubBuildingType = ESubBuildingType::LinearLink;
			SubBuildings[i].Direction *= -1;
			continue;
		}

		if (i == SubBuildings.Num() - 2)
		{
			SubBuildings[i].SubBuildingType = ESubBuildingType::LinearLink;
			continue;
		}

		if (BuildingData->UniqueMeshFrequency > 0)
		{
			if (i % BuildingData->UniqueMeshFrequency == 0)
			{
				SubBuildings[i].SubBuildingType = ESubBuildingType::LinearUnique;
				continue;
			}
		}
	}
}

