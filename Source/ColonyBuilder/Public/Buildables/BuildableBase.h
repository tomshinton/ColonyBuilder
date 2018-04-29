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

	UPROPERTY(VisibleAnywhere, Category = "Building Info")
	TArray<FSubBuilding> SubBuildings;

	void OnConstruction(const FTransform& Transform);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	UBuildingData* BuildingData;

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
	UUI_SelectionBox* SelectionWidget;
//ISelectionInterface

};
