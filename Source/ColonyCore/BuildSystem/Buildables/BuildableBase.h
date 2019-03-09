// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "Utils/DataTypes/BuildingDataTypes.h"
#include "Save/SavableInterface.h"
#include "Utils/DataTypes/SaveDataTypes.h"
#include "Core/Selection/SelectionInterface.h"

#include "BuildableBase.generated.h"

class UBuildingData;
class AVillagerController;
class ABaseVillager;

class USceneComponent;
class UStaticMeshComponent;
class UConstructionComponent;
class UGarrisonPoint;

class UUI_SelectionBox;

//////////////////////////////////////////////////////////////////////////
// Base actor for any actor that can be built via the Construction System
//////////////////////////////////////////////////////////////////////////

UCLASS(Blueprintable)
class COLONYCORE_API ABuildableBase : public AActor, public ISavableInterface, public ISelectionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildableBase();

	void OnConstruction(const FTransform& Transform);

	UFUNCTION(BlueprintPure, Category = Construction)
	UConstructionComponent* GetConstructionComponent() { return ConstructionComponent; }

	TWeakObjectPtr<UGarrisonPoint> GetGarrisonPoint() { return CachedGarrisonPoint; }

	UFUNCTION()
	virtual void EnableBuilding();

	void AddEmployee(ABaseVillager* InVillager);
	void AddResident(ABaseVillager* InVillager);

	bool HasVacancies() const;
	bool HasBoardingRoom() const;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* SceneRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UConstructionComponent* ConstructionComponent;

	UPROPERTY()
	TWeakObjectPtr<UGarrisonPoint> CachedGarrisonPoint;

	/** Was this buildable placed by design? If so, consider it a finished building on begin play */
	UPROPERTY(EditAnywhere, Category = "Building Info")
	bool IsPreplaced;

	UPROPERTY(VisibleAnywhere, Category = "Building Info")
	TArray<FSubBuilding> SubBuildings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	UBuildingData* BuildingData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool Preplaced;

	FGuid BuildingID;

	UPROPERTY()
	TArray<FGuid> RegisteredEmployees;

	UPROPERTY()
	TArray<FGuid> RegisteredResidents;

private:

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
