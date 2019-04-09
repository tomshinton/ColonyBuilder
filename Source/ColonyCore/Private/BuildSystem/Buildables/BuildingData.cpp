// ColonyBuilder Project, personal project by Tom Shinton

#include "BuildingData.h"
#include "ConstructorHelpers.h"
#include "GhostBase.h"
#include "GhostFF.h"

UBuildingData::UBuildingData() 
	: MaxBuilders(5)
{
	ConstructorHelpers::FObjectFinder<UMaterialInterface> ValidGhostRef(TEXT("MaterialInstanceConstant'/Game/Materials/PROTO/Ghosts/M_ValidGHost.M_ValidGhost'"));
	if (ValidGhostRef.Object)
	{
		ValidGhostMaterial = ValidGhostRef.Object;
	}
	ConstructorHelpers::FObjectFinder<UMaterialInterface> InvalidGhostRef(TEXT("MaterialInstanceConstant'/Game/Materials/PROTO/Ghosts/M_InvalidGhost.M_InvalidGhost'"));
	if (InvalidGhostRef.Object)
	{
		InvalidGhostMaterial = InvalidGhostRef.Object;
	}
}

void UBuildingData::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	switch (ConstructionMethod)
	{
	case EConstructionMethod::FireAndForget:
		GhostClass = AGhostFF::StaticClass();
		ShouldHideBaseMeshOnStartPlacement = false;
		break;
	case EConstructionMethod::Grid:
		GhostClass = AGhost::StaticClass();
		ShouldHideBaseMeshOnStartPlacement = true;
		break;
	case EConstructionMethod::Linear:
		GhostClass = AGhost::StaticClass();
		ShouldHideBaseMeshOnStartPlacement = true;
	}
}

FString UBuildingData::GetFullCategoryAsString()
{
	return BuildingCategory + "/" + BuildingSubcategory;
}
