// ColonyBuilder Project, personal project by Tom Shinton

#include "BuildComponent.h"

class UBuildingData;
class ARTSPlayerController;
class APlayerPawn;
class AGhost;
class UConstructionComponent;
class AColonyBuilderGameModeBase;

#include "Kismet/GameplayStatics.h"
#include "Utils/Libraries/GridUtils.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY(BuildComponentLog);

UBuildComponent::UBuildComponent()
	: CurrentBuildAction(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBuildComponent::StartBuildingFromClass(UBuildingData* InBuildingData)
{
	if (InBuildingData)
	{
		if (CurrentBuildAction)
		{
			CurrentBuildAction->CancelAction();
		}

		CurrentBuildAction = UBuildComponent::CreateBuildAction(InBuildingData, GetOwner());
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

//REFACTOR
TArray<FSubBuilding> UBuildComponent::BuildLinearPoints()
{
	TArray<FSubBuilding> OutPoints;

	if (UColonyGridSettings* GridSettings = GetMutableDefault<UColonyGridSettings>())
	{
		const int32 GridSize = GridSettings->GridSize;

		float XDelta = CurrRoundedMouseCoords.X - MouseLocationAtBuildingStart.X;
		float YDelta = CurrRoundedMouseCoords.Y - MouseLocationAtBuildingStart.Y;

		int32 XDeltaAsUnits = XDelta / GridSize;
		int32 YDeltaAsUnits = YDelta / GridSize;

		int8 XDir;
		int8 YDir;

		XDeltaAsUnits >= 0 ? XDir = 1 : XDir = -1;
		YDeltaAsUnits >= 0 ? YDir = 1 : YDir = -1;

		FVector NewPoint = MouseLocationAtBuildingStart;

		if (FMath::Abs(XDeltaAsUnits) > FMath::Abs(YDeltaAsUnits))
		{
			//Generate the points along the X
			for (int32 x = 0; x <= FMath::Abs(XDeltaAsUnits); x++)
			{
				NewPoint.X = MouseLocationAtBuildingStart.X + (GridSize * (x*XDir));
				FSubBuilding NewLocation(NewPoint, FVector2D(XDir, 0), EPointType::LinearPoint, ESubBuildingType::LinearBody);

				OutPoints.AddUnique(NewLocation);
			}
		}
		else
		{
			//Generate the points along the Y
			for (int32 y = 0; y <= FMath::Abs(YDeltaAsUnits); y++)
			{
				NewPoint.Y = MouseLocationAtBuildingStart.Y + (GridSize * (y*YDir));

				FSubBuilding NewLocation(NewPoint, FVector2D(0, YDir), EPointType::LinearPoint, ESubBuildingType::LinearBody);
				OutPoints.AddUnique(NewLocation);
			}
		}
	}

	return OutPoints;
}

//REFACTOR
TArray<FSubBuilding> UBuildComponent::BuildGridPoints()
{
	TArray<FSubBuilding> OutPoints;

	if (UColonyGridSettings* GridSettings = GetMutableDefault<UColonyGridSettings>())
	{
		const int32 GridSize = GridSettings->GridSize;

		float XDelta = CurrRoundedMouseCoords.X - MouseLocationAtBuildingStart.X;
		float YDelta = CurrRoundedMouseCoords.Y - MouseLocationAtBuildingStart.Y;

		int32 XDeltaAsUnits = XDelta / GridSize;
		int32 YDeltaAsUnits = YDelta / GridSize;

		int8 XDir;
		int8 YDir;

		XDeltaAsUnits >= 0 ? XDir = 1 : XDir = -1;
		YDeltaAsUnits >= 0 ? YDir = 1 : YDir = -1;

		for (int32 x = 0; x <= FMath::Abs(XDeltaAsUnits); x++)
		{
			for (int32 y = 0; y <= FMath::Abs(YDeltaAsUnits); y++)
			{
				FVector NewPoint = MouseLocationAtBuildingStart;

				NewPoint.X = NewPoint.X + (GridSize * (x*XDir));
				NewPoint.Y = NewPoint.Y + (GridSize * (y*YDir));

				FSubBuilding NewLocation(NewPoint, EPointType::GridPoint, ESubBuildingType::Body, FVector2D(FMath::Abs(x), FMath::Abs(y)), FVector2D(FMath::Abs(XDeltaAsUnits), FMath::Abs(YDeltaAsUnits)));
				OutPoints.AddUnique(NewLocation);
			}
		}
	}

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

/*void UBuildComponent::EndPlacement()
{
	TArray<AActor*> SpawnedBuildings;

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
				NewBuilding->BuildingID = FGuid::NewGuid();
				
				SpawnedBuildings.Add(NewBuilding);
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

					SpawnedBuildings.Add(NewSubBuilding);
				}

				if (SubBuilding.PointType == EPointType::LinearPoint)
				{
					ABuildableBase* NewSubBuilding = GetWorld()->SpawnActor<ABuildableBase>(BuildingData->BuildingClass, SubBuilding.Location, FVector(SubBuilding.Direction.X, SubBuilding.Direction.Y, 0).Rotation());
					NewSubBuilding->MeshComponent->SetStaticMesh(*BuildingData->SubBuildingMeshes.Find(SubBuilding.SubBuildingType));

					SpawnedBuildings.Add(NewSubBuilding);
				}
			}
		}

		for (AActor* SpawnedBuilding : SpawnedBuildings)
		{
			if (SpawnedBuilding)
			{
				TArray<UConstructionComponent*> ConstructionComponents;
				SpawnedBuilding->GetComponents<UConstructionComponent>(ConstructionComponents);
				for (UConstructionComponent* ConstructionComp : ConstructionComponents)
				{
					if (IConstructionInterface* ConstructionInterface = Cast<IConstructionInterface>(ConstructionComp))
					{
						ConstructionInterface->StartConstruction(BuildingData);
					}
				}
			}
		}

		SubBuildings.Empty();
}*/

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

