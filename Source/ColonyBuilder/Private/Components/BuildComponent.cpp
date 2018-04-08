// ColonyBuilder Project, personal project by Tom Shinton

#include "BuildComponent.h"

#include "BuildingData.h"

#include "RTSPlayerController.h"
#include "PlayerPawn.h"
#include "Ghost.h"

#include "Kismet/GameplayStatics.h"
#include "Utils/GridUtils.h"

#include "ColonyBuilderGameModeBase.h"
#include "BuildableBase.h"

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
	if (IsNewPlacement)
	{
		MouseLocationAtBuildingStart = CurrRoundedMouseCoords;
	}

	GetWorld()->GetTimerManager().SetTimer(BuildIntermediatePosTimer, this, &UBuildComponent::BuildIntermediatePositions, 0.1f, true);
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
			FSubBuilding NewLocation(NewPoint, FVector2D(XDir, 0), EPointType::LinearPoint, ESubBuildingType::SplineGeneral);

			OutPoints.AddUnique(NewLocation);
		}
	}
	else
	{
		//Generate the points along the Y
		for (int32 y = 0; y <= FMath::Abs(YDeltaAsUnits); y++)
		{
			NewPoint.Y = MouseLocationAtBuildingStart.Y + (AColonyBuilderGameModeBase::GridSize * (y*YDir));

			FSubBuilding NewLocation(NewPoint, FVector2D(0, YDir), EPointType::LinearPoint, ESubBuildingType::SplineGeneral);
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

			ESubBuildingType NewType = ESubBuildingType::Body;

			if (BuildingData->ShouldUseSpawnPadding)
			{
				if (y == FMath::Abs(YDeltaAsUnits) || x == FMath::Abs(XDeltaAsUnits) || x == 0 || y == 0)
				{
					NewType = ESubBuildingType::Edge;
				}

				if (PointIsInCorner(x, y, FMath::Abs(XDeltaAsUnits), FMath::Abs(XDeltaAsUnits)))
				{
					NewType = ESubBuildingType::Corner;
				}
			}

			FSubBuilding NewLocation(NewPoint, EPointType::GridPoint, NewType, FVector2D(FMath::Abs(x), FMath::Abs(y)), FVector2D(FMath::Abs(XDeltaAsUnits), FMath::Abs(YDeltaAsUnits)));
			OutPoints.AddUnique(NewLocation);
		}
	}

	return OutPoints;
}

bool UBuildComponent::PointIsInCorner(int32 PointX, int32 PointY, int32 MaxX, int32 MaxY)
{
	if (PointX == 0 && PointY == 0)
	{
		return true;
	}

	if (PointX == MaxX && PointY == MaxY)
	{
		return true;
	}

	if (PointX == 0 && PointY == MaxY)
	{
		return true;
	}

	if (PointX == MaxX && PointY == 0)
	{
		return true;
	}

	return false;
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
				//Moving in the Y
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
	//TArray<FSubBuilding> LocalPositions = GeneratedPositions;
	GetWorld()->GetTimerManager().ClearTimer(BuildIntermediatePosTimer);


	FVector CurrentGhostLoc;
	FRotator CurrentGhostRot;

	if (SpawnedGhost && SpawnedGhost->GetIsValid())
	{
		CurrentGhostLoc = SpawnedGhost->GetActorLocation();
		CurrentGhostRot = SpawnedGhost->GetActorRotation();
		
		SpawnedGhost->Destroy();
		FActorSpawnParameters SpawnParams;

		//Does this building type actually spawn a building at the end?
		if (BuildingData->BuildingClass)
		{
			ABuildableBase* NewBuilding = GetWorld()->SpawnActor<ABuildableBase>(BuildingData->BuildingClass, CurrentGhostLoc, CurrentGhostRot);
			NewBuilding->SubBuildings = SubBuildings;
		}

		for (FSubBuilding& SubBuilding : SubBuildings)
		{
			//Spawn plants and whatnot
			if (SubBuilding.SubBuildingType == ESubBuildingType::Body)
			{
				AGridBodyBase* NewSubBuilding = GetWorld()->SpawnActor<AGridBodyBase>(BuildingData->BodyClass, SubBuilding.Location, FRotator(0, 0, 0));
			}

			//Spawn linearly spawned actors
			if (SubBuilding.SubBuildingType == ESubBuildingType::SplineGeneral || SubBuilding.SubBuildingType == ESubBuildingType::SplineUnique)
			{
				ABuildableBase* NewSubBuilding = GetWorld()->SpawnActor<ABuildableBase>(BuildingData->BuildingClass, SubBuilding.Location, FVector(SubBuilding.Direction.X, SubBuilding.Direction.Y, 0).Rotation());

				NewSubBuilding->MeshComponent->SetStaticMesh(*BuildingData->SubBuildingMeshes.Find(SubBuilding.SubBuildingType));
			}
		}
	}

	SetEnabled(false);
	SubBuildings.Empty();
}

void UBuildComponent::CancelBuild()
{	
	SetEnabled(false);
}

void UBuildComponent::UpdateGhost()
{
	if (SpawnedGhost)
	{
		AlignAndOrientate();
		SpawnedGhost->UpdateGhost(CurrRoundedMouseCoords, SubBuildings);
	}
}

void UBuildComponent::ValidatePointTypesToUnique()
{
	for (int32 i = 0; i <= SubBuildings.Num() - 1; i++)
	{
		if (i == 0)
		{
			SubBuildings[i].SubBuildingType = ESubBuildingType::SplineUnique;
			continue;
		}

		if (i == SubBuildings.Num() - 1)
		{
			SubBuildings[i].SubBuildingType = ESubBuildingType::SplineUnique;
			continue;
		}

		if (BuildingData->UniqueMeshFrequency > 0)
		{
			if (i % BuildingData->UniqueMeshFrequency == 0)
			{
				SubBuildings[i].SubBuildingType = ESubBuildingType::SplineUnique;
				continue;
			}
		}
	}
}