// ColonyBuilder Project, personal project by Tom Shinton

#include "CellVisuals.h"
#include "ColonyGridSettings.h"

ACellVisuals::ACellVisuals()
	: VisualRoot(CreateDefaultSubobject<USceneComponent>(TEXT("VisualsRoot")))
	, Cells(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Visuals")))
	, CellMesh(nullptr)
	, DecalMat(nullptr)
	, SpawnedDecals()
{
	PrimaryActorTick.bCanEverTick = false;

	if (UColonyGridSettings* GridSettings = GetMutableDefault<UColonyGridSettings>())
	{
		if (UStaticMesh* LoadedCellMesh = Cast<UStaticMesh>(GridSettings->CellMesh.TryLoad()))
		{
			CellMesh = LoadedCellMesh;
			Cells->SetStaticMesh(LoadedCellMesh);
		}

		if (UMaterialInterface* LoadedCellMaterial = Cast<UMaterialInterface>(GridSettings->InvalidCellMaterial.TryLoad()))
		{
			Cells->SetMaterial(0, LoadedCellMaterial);
		}
	}
}

void ACellVisuals::Visualise(const TArray<FSubBuilding>& InPointsToRepresent)
{
	if (Cells)
	{
		for (const FSubBuilding& InPoint : InPointsToRepresent)
		{
			FTransform NewCell;
			NewCell.SetLocation(InPoint.Location);
			NewCell.SetRotation(FQuat());
			NewCell.SetScale3D(FVector(1.f, 1.f, 1.f));
			Cells->AddInstanceWorldSpace(NewCell);
		}
	}
}

