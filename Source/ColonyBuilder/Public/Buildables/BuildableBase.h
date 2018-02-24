// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"

#include "BuildableBase.generated.h"

class UBuildingData;

USTRUCT(BlueprintType)
struct FIntermediateBuildingLocation
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Building Info")
	FVector Location;

	bool operator==(const FIntermediateBuildingLocation& OtherLocation) const
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
class COLONYBUILDER_API ABuildableBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildableBase();

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* SceneRoot;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Building Info")
	TArray<FIntermediateBuildingLocation> IntermediateBuildingLocations;

	void OnConstruction(const FTransform& Transform);

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UBuildingData* BuildingData;

	
	
};
