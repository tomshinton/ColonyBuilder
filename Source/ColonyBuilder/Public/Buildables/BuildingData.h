// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "BuildableBase.h"

#include "BuildingData.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EConstructionMethod : uint8 
{
	FireAndForget	UMETA(DisplayName = "Fire and Forget"),
	Grid			UMETA(DisplayName = "Grid"),
	Linear			UMETA(DisplayName = "Linear Construction")
};

UCLASS()
class COLONYBUILDER_API UBuildingData : public UDataAsset
{
	GENERATED_BODY()

	UBuildingData();


public:
	UPROPERTY(EditAnywhere, Category = "Readability")
	FName BuildingName;
	
	UPROPERTY(EditAnywhere, Category = "Spawning | Data")
	TSubclassOf<ABuildableBase> BuildingClass;

	UPROPERTY(EditAnywhere, Category = "Rendering | Meshes")
	UStaticMesh* BuildingBaseMesh;

	UPROPERTY(EditAnywhere, Category = "Spawning | Actions")
	EConstructionMethod ConstructionMethod;

	UPROPERTY(EditAnywhere, Category = "Spawning | Actions")
	bool ShouldEndConstructionOnPlacement;

	UPROPERTY(EditAnywhere, Category = "Rendering | Materials")
	UMaterialInterface* ValidGhostMaterial;

	UPROPERTY(EditAnywhere, Category = "Rendering | Materials")
	UMaterialInterface* InvalidGhostMaterial;

	UPROPERTY(EditAnywhere, Category = "UI")
	UMaterialInterface* BuildingIcon;

	UPROPERTY(EditAnywhere, Category = "Spawning | Dimensions")
	FVector2D MaxDimentions;

	UPROPERTY(EditAnywhere, Category = "Spawning | Dimensions")
	FVector2D MinDimentions;

	UPROPERTY(EditAnywhere, Category = "Organisation")
	FString BuildingCategory;

	UPROPERTY(EditAnywhere, Category = "Organisation")
	FString BuildingSubcategory;

public:
	FString GetFullCategoryAsString();

};
