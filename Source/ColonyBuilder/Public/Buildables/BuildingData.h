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
	
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<ABuildableBase> BuildingClass;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	UStaticMesh* BuildingBaseMesh;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	EConstructionMethod ConstructionMethod;

	UPROPERTY(EditAnywhere, Category = "Rendering | Materials")
	UMaterialInterface* ValidGhostMaterial;

	UPROPERTY(EditAnywhere, Category = "Rendering | Materials")
	UMaterialInterface* InvalidGhostMaterial;

	UPROPERTY(EditAnywhere, Category = "UI")
	UMaterialInterface* BuildingIcon;

};
