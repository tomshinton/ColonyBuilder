// ColonyBuilder Project, personal project by Tom Shinton

#include "Ghost.h"


// Sets default values
AGhost::AGhost()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	RootComponent = MeshComp;

}

void AGhost::SetGhost(UStaticMesh* GhostMesh)
{
	MeshComp->SetStaticMesh(GhostMesh);
}

