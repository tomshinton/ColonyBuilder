// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include <Engine/DataAsset.h>

class UDataAsset;
class AGridBodyBase;
class ABuildableBase;
class UUI_SelectionBox;
class UPointValidationRules;
class AGhost;

#include "BuildingData.generated.h"

UCLASS()
class COLONYCORE_API UBuildingData : public UDataAsset
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

#pragma endregion 
	
#pragma region Rendering

	UPROPERTY(EditAnywhere, Category = "Rendering | Meshes")
	TMap<ESubBuildingType, UStaticMesh*> SubBuildingMeshes;

	UPROPERTY(EditAnywhere, Category = "Rendering | Meshes")
	int32 UniqueMeshFrequency;

	UPROPERTY(EditAnywhere, Category = "Rendering | Meshes")
	bool ShouldHideBaseMeshOnStartPlacement;

#pragma endregion 

#pragma region Materials
	UPROPERTY(EditAnywhere, Category = "Rendering | Materials")
	UMaterialInterface* ValidGhostMaterial;

	UPROPERTY(EditAnywhere, Category = "Rendering | Materials")
	UMaterialInterface* InvalidGhostMaterial;

	UPROPERTY(EditAnywhere, Category = "Rendering | Materials")
	UMaterialInterface* OnSelectionMaterial;
#pragma endregion 

#pragma region UI
	UPROPERTY(EditAnywhere, Category = "UI")
	UMaterialInterface* BuildingIcon;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUI_SelectionBox> SelectionWidget;
#pragma endregion 

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
	TSubclassOf<AGhost> GhostClass;

	UPROPERTY(EditAnywhere, Category = "Spawning | Classes")
	TSubclassOf<AGridBodyBase> BodyClass;

	UPROPERTY(EditAnywhere, Category = "Spawning | Dimensions")
	FVector2D MaxDimensions;

	UPROPERTY(EditAnywhere, Category = "Spawning | Dimensions")
	FVector2D MinDimentions;

#pragma endregion 

#pragma region Construction
	UPROPERTY(EditAnywhere, Category = "Spawning | Construction")
	float TotalBuildTime;

	UPROPERTY(EditAnywhere, Category = "Spawning | Construction")
	int32 MaxBuilders;

#pragma endregion 

#pragma region Professions
	UPROPERTY(EditAnywhere, Category = "Professions | Construction")
	TArray<TSubclassOf<UProfessionBase>> Professions;

	UPROPERTY(EditAnywhere, Category = "Professions | Employment")
	int32 MaxEmployees;
#pragma endregion

#pragma region Residence
	UPROPERTY(EditAnywhere, Category = "Residence")
	int32 MaxResidents;
#pragma endregion
	

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

public:
	FString GetFullCategoryAsString();
};
