// ColonyBuilder Project, personal project by Tom Shinton

#include "GridBodyBase.h"
#include "ColonyBuilderGameModeBase.h"

const FName AGridBodyBase::MeshName = FName("MeshName");

FBuildingSaveData AGridBodyBase::GetBuildingSaveData()
{
	FBuildingSaveData NewData(GetClass(), GetActorTransform(), nullptr);
	return NewData;
}

void AGridBodyBase::LoadBuildingSaveData(FBuildingSaveData LoadedData)
{}

AGridBodyBase::AGridBodyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Grid Body Root"));
	RootComponent = SceneRoot;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(AGridBodyBase::MeshName);
	StaticMeshComp->SetupAttachment(SceneRoot);

	GridBodyCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Body Collision"));
	GridBodyCollision->SetupAttachment(SceneRoot);
	GridBodyCollision->SetBoxExtent(FVector(AColonyBuilderGameModeBase::GridSize / 2, AColonyBuilderGameModeBase::GridSize / 2, AColonyBuilderGameModeBase::GridSize / 2), true);
}