// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "AI/Pawns/BaseVillager.h"
#include "GarrisonPoint.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UBoxComponent;

//////////////////////////////////////////////////////////////////////////
// Essentially a doorway - this is where pawns enter and exit a building or structure
//////////////////////////////////////////////////////////////////////////

UCLASS( ClassGroup=(BuildingComponents), meta=(BlueprintSpawnableComponent), Blueprintable )
class COLONYCORE_API UGarrisonPoint : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGarrisonPoint();

	UPROPERTY(EditAnywhere, Category = Component)
	UStaticMeshComponent* MeshComponent;
	
	bool IsPawnGarrisoned(ABaseVillager* InPawn) const { return GarrisonedVillagers.Contains(InPawn); };

	void Ungarrison(ABaseVillager* BaseVillager);
	void Garrison(ABaseVillager* BaseVillager);

private:

	UPROPERTY()
	TArray<ABaseVillager*> GarrisonedVillagers;
};
