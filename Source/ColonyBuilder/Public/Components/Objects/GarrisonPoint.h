// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "BaseVillager.h"
#include "GarrisonPoint.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UBoxComponent;

//////////////////////////////////////////////////////////////////////////
// Essentially a doorway - this is where pawns enter and exit a building or structure
//////////////////////////////////////////////////////////////////////////

UCLASS( ClassGroup=(BuildingComponents), meta=(BlueprintSpawnableComponent), Blueprintable )
class COLONYBUILDER_API UGarrisonPoint : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGarrisonPoint();

	UPROPERTY(EditAnywhere, Category = Component)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, Category = Component)
	UBoxComponent* OverlapComponent;
	
	void AddExpectedPawn(ABaseVillager* InPawn) { ExpectedGarrisonedVillagers.AddUnique(InPawn); }
	bool IsPawnGarrisoned(ABaseVillager* InPawn) const { return GarrisonedVillagers.Contains(InPawn); };

	//This can be public as it has BT hookins
	void Ungarrison(ABaseVillager* BaseVillager);
private:

	UPROPERTY()
	TArray<ABaseVillager*> ExpectedGarrisonedVillagers;

	UPROPERTY()
	TArray<ABaseVillager*> GarrisonedVillagers;

	UFUNCTION()
	void AttemptGarrison(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
