// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "RTSBaseComp.h"
#include "BuildableBase.h"

#include "BuildComponent.generated.h"

class UBuildingData;
class AGhost;

class ARTSPlayerController;
class APlayerPawn;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COLONYBUILDER_API UBuildComponent : public URTSBaseComp
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBuildComponent();

	void BeginPlay();

	//Setters//
	virtual void SetEnabled(bool InEnabled) override;

	UFUNCTION()
	void UpdateMouseCoords(FVector InCurrMouseCoords, FVector InRoundedMouseCoords);
	void RotatePlacement();

	void StartBuildingFromClass(UBuildingData* BuildingData);
	void StartPlacement();
	void EndPlacement();
	void CancelBuild();

	TArray<FIntermediateBuildingLocation> BuildLinearPoints();
	TArray<FIntermediateBuildingLocation> BuildGridPoints();

	FTimerHandle BuildIntermediatePosTimer;
	void BuildIntermediatePositions();
	TArray<FIntermediateBuildingLocation> GeneratedPositions;

	UBuildingData* CurrentBuildingData;

private:

	FVector CurrMouseCoords;
	FVector CurrRoundedMouseCoords;
	FVector MouseLocationAtBuildingStart;
	void UpdateGhostLocation();
	AGhost* SpawnedGhost;

	float RotationRate = 45.f;

	ARTSPlayerController* ControllerRef;
	
};
