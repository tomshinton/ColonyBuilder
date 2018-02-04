// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "BuildableBase.generated.h"

class UBuildingData;


UCLASS()
class COLONYBUILDER_API ABuildableBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildableBase();

	UBuildingData* BuildingData;

	
	
};
