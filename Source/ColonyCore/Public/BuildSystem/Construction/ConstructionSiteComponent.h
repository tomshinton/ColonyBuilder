// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

class UBoxComponent;
class UBillboardComponent;

#include "ConstructionSiteComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COLONYCORE_API UConstructionSiteComponent : public USceneComponent
{
	GENERATED_BODY()

public:	

	UConstructionSiteComponent();

	UBoxComponent* CollisionComponent;

	UBillboardComponent* Sprite;
};
