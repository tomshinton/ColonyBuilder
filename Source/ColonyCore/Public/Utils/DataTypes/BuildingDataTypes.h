// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

class FValidationRunner;

#include "BuildingDataTypes.generated.h"

UENUM(BlueprintType)
enum class EInvalidReason : uint8
{
	NoPointRules	UMETA(DisplayName = "No Display Rules"),
	PointIsTooHigh	UMETA(DisplayName = "Point it too high"),
	PointIsTooLow	UMETA(DisplayName = "Point is too low"),
	GridIsTooBig	UMETA(DisplayName = "Grid is too big"),
	GridIsTooSmall	UMETA(DisplayName = "Grid is too small"),
	SurfaceTooSteep	UMETA(DisplayName = "Surface is too steep"),
	NoWorldContext	UMETA(DisplayName = "No World Context"),
	IllegalOverlap	UMETA(DisplayName = "Illegal Overlap at Point"),
	LegalOverlap	UMETA(DisplayName = "Legal Overlap at Point")
};

//What kind of method a building this construction uses - used for telling what mode the component should run in
UENUM(BlueprintType)
enum class EConstructionMethod : uint8
{
	FireAndForget	UMETA(DisplayName = "Fire and Forget"),
	Grid			UMETA(DisplayName = "Grid"),
	Linear			UMETA(DisplayName = "Linear Construction")
};

UENUM(BlueprintType)
enum class EConstructionStage : uint8
{
	AwaitingResources,
	Construction,
	Upgrading,
	Finished
};

//By what means was this generated - linearly, or through the grid generation
UENUM(BlueprintType)
enum class EPointType : uint8
{
	GridPoint	UMETA(DisplayName = "Grid Point"),
	LinearPoint	UMETA(DisplayName = "Linear Point"),
	BuildingPoint	UMETA(DisplayName = "Building Point")
};

//What kind of sub building is this? Edges, corners or grids
UENUM(BlueprintType)
enum class ESubBuildingType : uint8
{
	Corner	UMETA(DisplayName = "Corner"),
	Edge	UMETA(DisplayName = "Edge"),
	Body	UMETA(DisplayName = "Body"),
	LinearTerminator	UMETA(DisplayName = "Linear Terminator"),
	LinearLink UMETA(DisplayName = "Linear Link"),
	LinearBody	UMETA(DisplayName = "Linear Body"),
	LinearUnique	UMETA(DisplayName = "Linear Unique")
};

//All info associated with a sub building - location, type, maxX and Y etc etc
USTRUCT(BlueprintType)
struct FSubBuilding
{
	GENERATED_BODY()

	FSubBuilding(){}

	FSubBuilding(const FVector& InLocation, const EPointType& InPointType) :
		Location(InLocation)
		, PointType(InPointType)
	{}

	//For linear points
	FSubBuilding(const FVector& InLocation, const FVector2D& InDirection, const EPointType& InPointType, const ESubBuildingType& InSubBuildingType) :
	Location(InLocation)
	, Direction(InDirection)
	, PointType(InPointType)
	, SubBuildingType(InSubBuildingType)
	{}

	//For grid points
	FSubBuilding(const FVector& InLocation, const EPointType& InPointType, const ESubBuildingType& InSubBuildingType, const FVector2D& InCurrCoord, const FVector2D& InMaxCoord) :
		Location(InLocation)
		, PointType(InPointType)
		, SubBuildingType(InSubBuildingType)
		, CurrCoord(InCurrCoord)
		, MaxCoord(InMaxCoord)
	{}
	
public: 

	void SetHitResult(const FHitResult& InHitResult)
	{
		HitResult = InHitResult;
	}

	bool IsValidSubBuilding() const 
	{
		return HitResult.IsSet();
	}

	void AddFailureReason(const EInvalidReason& InNewReason)
	{
		PlacementFailureReasons.AddUnique(InNewReason);
	}

	FVector Location;
	FVector2D Direction;

	FVector LocationNormal;

	bool IsValidPoint;

	FVector2D CurrCoord;
	FVector2D MaxCoord;

	EPointType PointType;
	ESubBuildingType SubBuildingType;

	TOptional<FHitResult> HitResult;

	TArray<EInvalidReason> PlacementFailureReasons;

	bool operator==(const FSubBuilding& OtherLocation) const
	{
		return Location == OtherLocation.Location;
	}
};

UCLASS()
class UBuildingDataTypes : public UObject
{
	GENERATED_BODY()	
};
