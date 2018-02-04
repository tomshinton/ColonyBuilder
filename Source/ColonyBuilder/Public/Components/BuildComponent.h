// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "BuildComponent.generated.h"

class UBuildingData;
class AGhost;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COLONYBUILDER_API UBuildComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBuildComponent();

	void BeginPlay();

	//Getters//
	bool GetEnabled() { return IsEnabled; }
	//Setters//
	void SetEnabled(bool NewEnabled) { IsEnabled = NewEnabled; }
	UFUNCTION()
	void UpdateMouseCoords(FVector InCurrMouseCoords, FVector InRoundedMouseCoords);

	void StartBuildingFromClass(UBuildingData* BuildingData);
	void CancelBuild();

	UBuildingData* CurrentBuildingData;

private:
	
	bool IsEnabled;
	FVector CurrMouseCoords;
	FVector CurrRoundedMouseCoords;
	void UpdateGhostTrans();
	AGhost* SpawnedGhost;
	
};
