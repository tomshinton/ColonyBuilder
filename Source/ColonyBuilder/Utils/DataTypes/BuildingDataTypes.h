// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BuildingDataTypes.generated.h"

//What kind of method a building this construction uses - used for telling what mode the component should run in
UENUM(BlueprintType)
enum class EConstructionMethod : uint8
{
	FireAndForget	UMETA(DisplayName = "Fire and Forget"),
	Grid			UMETA(DisplayName = "Grid"),
	Linear			UMETA(DisplayName = "Linear Construction")
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
	GENERATED_USTRUCT_BODY()

	FSubBuilding() {}

	FSubBuilding(FVector Location, EPointType PointType) :
		Location(Location)
		, PointType(PointType)
	{}

	//For linear points
	FSubBuilding(FVector Location, FVector2D Direction, EPointType PointType, ESubBuildingType SubBuildingType) :
	Location(Location)
	, Direction(Direction)
	, PointType(PointType)
	, SubBuildingType(SubBuildingType)
	{}

	//For grid points
	FSubBuilding(FVector Location, EPointType PointType, ESubBuildingType SubBuildingType, FVector2D CurrCoord, FVector2D MaxCoord) :
		Location(Location)
		, PointType(PointType)
		, SubBuildingType(SubBuildingType)
		, CurrCoord(CurrCoord)
		, MaxCoord(MaxCoord)
	{}

	FVector Location;
	FVector2D Direction;

	FVector LocationNormal;

	bool IsValidPoint;

	FVector2D CurrCoord;
	FVector2D MaxCoord;

	EPointType PointType;
	ESubBuildingType SubBuildingType;

	UClass* SurfaceReference;

	bool operator==(const FSubBuilding& OtherLocation) const
	{
		return Location == OtherLocation.Location;
	}

	FString ToStringFromInts()
	{
		FString OutString = "X: " + FString::FromInt(FMath::CeilToInt(Location.X)) + ", Y: " + FString::FromInt(FMath::CeilToInt(Location.Y));
		return OutString;
	}
};

UCLASS()
class COLONYBUILDER_API UBuildingDataTypes : public UObject
{
	GENERATED_BODY()	
};
