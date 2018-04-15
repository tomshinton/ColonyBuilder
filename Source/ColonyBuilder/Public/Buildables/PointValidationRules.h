// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PointValidationRules.generated.h"

/**
 * 
 */
UCLASS()
class COLONYBUILDER_API UPointValidationRules : public UDataAsset
{
	GENERATED_BODY()

	UPointValidationRules();

public:
	UPROPERTY(EditAnywhere, Category = "Location Checks | Height")
	float MinPointHeight = 0;

	UPROPERTY(EditAnywhere, Category = "Location Checks | Height")
	float MaxPointHeight = 200;
	
	UPROPERTY(EditAnywhere, Category = "Location Checks | Coords")
	int32 MinX;

	UPROPERTY(EditAnywhere, Category = "Location Checks | Coords")
	int32 MinY;

	UPROPERTY(EditAnywhere, Category = "Location Checks | Coords")
	int32 MaxX;

	UPROPERTY(EditAnywhere, Category = "Location Checks | Coords")
	int32 MaxY;

	UPROPERTY(EditAnywhere, Category = "Location Checks | Angle")
	float MaxPitch;

	UPROPERTY(EditAnywhere, Category = "Location Checks | Overlaps")
	TArray<UClass*> AllowedOverlapClasses;

	UPROPERTY(EditAnywhere, Category = "Location Checks | Overlaps")
	bool AllowSameTypeOverlaps;
};
