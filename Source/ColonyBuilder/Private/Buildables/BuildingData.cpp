// ColonyBuilder Project, personal project by Tom Shinton

#include "BuildingData.h"
#include "ConstructorHelpers.h"

UBuildingData::UBuildingData()
{
	ConstructorHelpers::FObjectFinder<UMaterialInterface> ValidGhostRef(TEXT("MaterialInstanceConstant'/Game/Materials/Placeholder/Ghosts/M_ValidGHost.M_ValidGhost'"));
	if (ValidGhostRef.Object)
	{
		ValidGhostMaterial = ValidGhostRef.Object;
	}
	ConstructorHelpers::FObjectFinder<UMaterialInterface> InvalidGhostRef(TEXT("MaterialInstanceConstant'/Game/Materials/Placeholder/Ghosts/M_InvalidGhost.M_InvalidGhost'"));
	if (InvalidGhostRef.Object)
	{
		InvalidGhostMaterial = InvalidGhostRef.Object;
	}
}

FString UBuildingData::GetFullCategoryAsString()
{
	return BuildingCategory + "/" + BuildingSubcategory;
}
