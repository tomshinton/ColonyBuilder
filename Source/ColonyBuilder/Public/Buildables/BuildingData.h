// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "BuildableBase.h"

#include "BuildingData.generated.h"

/**
 * 
 */
UCLASS()
class COLONYBUILDER_API UBuildingData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Readability")
	FName BuildingName;
	
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<ABuildableBase> BuildingClass;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	UStaticMesh* GhostMesh;

};
