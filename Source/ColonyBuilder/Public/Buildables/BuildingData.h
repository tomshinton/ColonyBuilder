// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "Utils/DataTypes/BuildingDataTypes.h"
#include "GridBodyBase.h"
#include "BuildableBase.h"
#include "UI_SelectionBox.h"

#include "BuildingData.generated.h"

class UPointValidationRules;

UCLASS()
class COLONYBUILDER_API UBuildingData : public UDataAsset
{
	GENERATED_BODY()

	UBuildingData();

public:

#pragma region Readability

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Readability")
	FName BuildingName;

	UPROPERTY(EditAnywhere, Category = "Readability")
	FString BuildingCategory;

	UPROPERTY(EditAnywhere, Category = "Readability")
	FString BuildingSubcategory;

#pragma endregion Readability
	
#pragma region Rendering

	UPROPERTY(EditAnywhere, Category = "Rendering | Meshes")
	UStaticMesh* BuildingBaseMesh;

	UPROPERTY(EditAnywhere, Category = "Rendering | Meshes")
	TMap<ESubBuildingType, UStaticMesh*> SubBuildingMeshes;

	UPROPERTY(EditAnywhere, Category = "Rendering | Meshes")
	int32 UniqueMeshFrequency;

	UPROPERTY(EditAnywhere, Category = "Rendering | Meshes")
	bool ShouldHideBaseMeshOnStartPlacement;

#pragma endregion Rendering

#pragma region Materials
	UPROPERTY(EditAnywhere, Category = "Rendering | Materials")
	UMaterialInterface* ValidGhostMaterial;

	UPROPERTY(EditAnywhere, Category = "Rendering | Materials")
	UMaterialInterface* InvalidGhostMaterial;

	UPROPERTY(EditAnywhere, Category = "Rendering | Materials")
	UMaterialInterface* OnSelectionMaterial;
#pragma endregion Materials

#pragma region UI
	UPROPERTY(EditAnywhere, Category = "UI")
	UMaterialInterface* BuildingIcon;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUI_SelectionBox> SelectionWidget;
#pragma endregion UI

#pragma region Spawning
	UPROPERTY(EditAnywhere, Category = "Spawning | Actions")
	bool ShouldUseSpawnPadding;

	UPROPERTY(EditAnywhere, Category = "Spawning | Rules")
	UPointValidationRules* PointRules;

	UPROPERTY(EditAnywhere, Category = "Spawning | Actions")
	EConstructionMethod ConstructionMethod;

	UPROPERTY(EditAnywhere, Category = "Spawning | Classes")
	TSubclassOf<ABuildableBase> BuildingClass;

	UPROPERTY(EditAnywhere, Category = "Spawning | Classes")
	TSubclassOf<AGridBodyBase> BodyClass;

	UPROPERTY(EditAnywhere, Category = "Spawning | Dimensions")
	FVector2D MaxDimentions;

	UPROPERTY(EditAnywhere, Category = "Spawning | Dimensions")
	FVector2D MinDimentions;

#pragma endregion Spawning

#pragma region Construction
	UPROPERTY(EditAnywhere, Category = "Spawning | Construction")
	float TotalBuildTime;
#pragma endregion Construction

public:
	FString GetFullCategoryAsString();

};
