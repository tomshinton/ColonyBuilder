// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "Utils/DataTypes/BuildingDataTypes.h"
#include "Core/Components/RTSBaseComp.h"
#include "../Buildables/BuildingData.h"

class UBuildAction;
class UBuildActionFF;
class UBuildActionGrid;
class UBuildActionLinear;

class AGhost;
class ARTSPlayerController;
class APlayerPawn;

#include "BuildComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(BuildComponentLog, Error, All);

UCLASS()
class COLONYCORE_API UBuildComponent : public URTSBaseComp
{
	GENERATED_BODY()

public:	

	UBuildComponent();

	UFUNCTION(BlueprintCallable, Category = "Building")
	void StartBuildingFromClass(UBuildingData* BuildingData);

	void BuildIntermediatePositions();
	void AlignAndOrientate();
	void ValidatePointTypesToUnique();
	
	TArray<FSubBuilding> BuildFFPoints();
	TArray<FSubBuilding> BuildLinearPoints();
	TArray<FSubBuilding> BuildGridPoints();

	FTimerHandle BuildIntermediatePosTimer;
	
	UBuildingData* BuildingData;
	TArray<FSubBuilding> SubBuildings;

private:

	FVector CurrMouseCoords;
	FVector CurrRoundedMouseCoords;
	FVector MouseLocationAtBuildingStart;
	void UpdateGhost();
	AGhost* SpawnedGhost;

	float RotationRate = 45.f;

	bool HasStartedBuilding;


public:

	UPROPERTY()
	UBuildAction* CurrentBuildAction;

	UFUNCTION()	void RotatePlacement() { if (CurrentBuildAction) { CurrentBuildAction->RotatePlacement(); }};
	UFUNCTION()	void StartBuild() { if (CurrentBuildAction) { CurrentBuildAction->StartAction(); }};
	UFUNCTION() void EndBuild() { if (CurrentBuildAction) { CurrentBuildAction->CompleteAction(); }};
	UFUNCTION() void CancelBuild() { if (CurrentBuildAction) { CurrentBuildAction->CancelAction(); }};

	static UBuildAction* CreateBuildAction(UBuildingData* InBuildingData, UObject* InOuter)
	{
		if (InBuildingData)
		{
			const EConstructionMethod ConstructionMethod = InBuildingData->ConstructionMethod;
			UBuildAction* NewBuildAction = nullptr;
			switch (ConstructionMethod)
			{
			case EConstructionMethod::FireAndForget:
				NewBuildAction = NewObject<UBuildActionFF>(InOuter);
				break;
			case EConstructionMethod::Grid:
				NewBuildAction = NewObject<UBuildActionGrid>(InOuter);
				break;
			case EConstructionMethod::Linear:
				NewBuildAction = NewObject<UBuildActionLinear>(InOuter);
				break;
			}

			if (NewBuildAction)
			{
				NewBuildAction->SetBuildingData(InBuildingData);
				//Optional replacement for BeginPlay, consider as an Init func
				NewBuildAction->OnBuild();
			}

			return NewBuildAction;
		}
		return nullptr;
	}	
};
