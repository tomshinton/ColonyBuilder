// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"

#include "Utils/DataTypes/BuildingDataTypes.h"

#include "Save/SavableInterface.h"
#include "SelectionInterface.h"

#include "Utils/DataTypes/SaveDataTypes.h"
#include "UI_SelectionBox.h"
#include "Construction/ConstructionComponent.h"

#include "BuildableBase.generated.h"

class UBuildingData;

UCLASS()
class COLONYBUILDER_API ABuildableBase : public AActor,	public ISavableInterface, public ISelectionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildableBase();

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* SceneRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UConstructionComponent* ConstructionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Building Info")
	TArray<FSubBuilding> SubBuildings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	UBuildingData* BuildingData;

	void OnConstruction(const FTransform& Transform);

	UFUNCTION(BlueprintPure, Category = Construction)
	UConstructionComponent* GetConstructionComponent() { return ConstructionComponent; }

	UFUNCTION()
	virtual void EnableBuilding();

private:
	int32 BuildingID;

//ISavableInterface
public:
	virtual FBuildingSaveData GetBuildingSaveData() override;
	virtual void LoadBuildingSaveData(FBuildingSaveData LoadedData) override;
//ISavableInterface

//ISelectionInterface
public:
	virtual void OnReceiveHover() override;
	virtual void OnEndHover() override;
	virtual void OnSelect() override;
	virtual void OnEndSelect() override;
	bool IsSelected;

	UPROPERTY()
	UUI_SelectionBox* SelectionWidget;
//ISelectionInterface

};
