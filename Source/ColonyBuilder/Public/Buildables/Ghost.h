// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Ghost.generated.h"

class UBuildingData;

UCLASS()
class COLONYBUILDER_API AGhost : public AActor
{
	GENERATED_BODY()

	UStaticMeshComponent* MeshComp;
	UBoxComponent* BoundsComp;
	
public:	
	// Sets default values for this actor's properties
	AGhost();
	static const float ValidationCheckFreq;
	
	void OnConstruction(const FTransform& Transform);

	void SetValid(bool InValidState);
	bool GetIsValid() { return IsValid; }
	
	
	void SetGhostInfo(UBuildingData* InBuildingData);
	UBuildingData* BuildingData;
	FTimerHandle CheckPlacementTimer;

	void CheckPlacement();
	void GetObstructingActors(TArray<AActor*>& ObstructingActors);
	
private:
	bool IsValid;

};
