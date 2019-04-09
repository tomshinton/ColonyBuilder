#pragma once

#include "ColonyGridSettings.generated.h"

//////////////////////////////////////////////////////////////////////////
// All info pertaining to the grid and how its displayed
//////////////////////////////////////////////////////////////////////////

UCLASS(config = Game, defaultconfig)
class COLONYCORE_API UColonyGridSettings : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, config, Category = "Grid | Dimensions", meta = (ClampMin = "0", UIMin = "2048"))
	int32 GridSize;

	UPROPERTY(EditAnywhere, config, Category = "Grid | Visuals", meta = (AllowedClasses = "StaticMesh"))
	FStringAssetReference CellMesh;

	UPROPERTY(EditAnywhere, config, Category = "Grid | Visuals", meta = (AllowedClasses = "Material"))
	FStringAssetReference ValidCellMaterial;

	UPROPERTY(EditAnywhere, config, Category = "Grid | Visuals", meta=(AllowedClasses = "Material"))
	FStringAssetReference InvalidCellMaterial;

	UPROPERTY(EditAnywhere, config, Category = "Grid | Visuals", meta = (AllowedClasses = "Material"))
	FStringAssetReference CellDecalMaterial;
};
