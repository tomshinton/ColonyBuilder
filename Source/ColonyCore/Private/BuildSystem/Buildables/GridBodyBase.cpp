// ColonyBuilder Project, personal project by Tom Shinton

#include "GridBodyBase.h"
#include "Core/GameMode/ColonyBuilderGameModeBase.h"
#include "ColonyGridSettings.h"

const FName AGridBodyBase::MeshName = FName("MeshName");

FBuildingSaveData AGridBodyBase::GetBuildingSaveData()
{
	FBuildingSaveData NewData(BuildingID, GetClass(), GetActorTransform(), nullptr);
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

	if (UColonyGridSettings* GridSettings = GetMutableDefault<UColonyGridSettings>())
	{
		const int32 GridSize = GridSettings->GridSize;
		GridBodyCollision->SetBoxExtent(FVector(GridSize *0.5f, GridSize *0.5f, GridSize *0.5f), true);
	}
}