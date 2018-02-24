// ColonyBuilder Project, personal project by Tom Shinton

#include "BuildingData.h"
#include "ConstructorHelpers.h"

UBuildingData::UBuildingData()
{
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> ValidGhostMaterial_Ref(TEXT("/Game/Materials/Placeholder/FlatColours/M_FlatColour_ValidGhost.M_FlatColour_ValidGhost"));
	if (ValidGhostMaterial_Ref.Object)
	{
		ValidGhostMaterial = ValidGhostMaterial_Ref.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> InvalidGhostMaterial_Ref(TEXT("/Game/Materials/Placeholder/FlatColours/M_FlatColour_InvalidGhost.M_FlatColour_InvalidGhost"));
	if (InvalidGhostMaterial_Ref.Object)
	{
		InvalidGhostMaterial = InvalidGhostMaterial_Ref.Object;
	}
}

FString UBuildingData::GetFullCategoryAsString()
{
	return BuildingCategory + "/" + BuildingSubcategory;
}
