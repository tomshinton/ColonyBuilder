// ColonyBuilder Project, personal project by Tom Shinton

#include "BuildingData.h"
#include "ConstructorHelpers.h"

UBuildingData::UBuildingData() :
	MaxBuilders(5)

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

	if (ConstructionMethod == EConstructionMethod::Grid || ConstructionMethod == EConstructionMethod::Linear)
	{
		ShouldHideBaseMeshOnStartPlacement = true;
	}
	else
	{
		ShouldHideBaseMeshOnStartPlacement = false;
	}
}

FString UBuildingData::GetFullCategoryAsString()
{
	return BuildingCategory + "/" + BuildingSubcategory;
}
