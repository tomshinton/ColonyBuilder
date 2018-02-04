// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ghost.generated.h"

UCLASS()
class COLONYBUILDER_API AGhost : public AActor
{
	GENERATED_BODY()

	UStaticMeshComponent* MeshComp;
	
public:	
	// Sets default values for this actor's properties
	AGhost();
	
	void SetGhost(UStaticMesh* GhostMesh);
	
	
};
