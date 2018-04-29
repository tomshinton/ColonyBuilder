// ColonyBuilder Project, personal project by Tom Shinton

#include "BuildableBase.h"
#include "BuildingData.h"
#include "UserWidget.h"

// Sets default values
ABuildableBase::ABuildableBase()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Building Root"));
	RootComponent = SceneRoot;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Building Static Mesh"));
	MeshComponent->SetCollisionProfileName("BlockAllDynamic");
	MeshComponent->SetupAttachment(SceneRoot);
	MeshComponent->bGenerateOverlapEvents = true;
}

void ABuildableBase::OnConstruction(const FTransform& Transform)
{
	if (BuildingData)
	{
		MeshComponent->SetStaticMesh(BuildingData->BuildingBaseMesh);

		SetFolderPath(FName(*BuildingData->GetFullCategoryAsString()));
	}
}

//ISavableInterface
FBuildingSaveData ABuildableBase::GetBuildingSaveData()
{
	FBuildingSaveData NewData(GetClass(), GetActorTransform(), MeshComponent->GetStaticMesh());
	return NewData;
}

void ABuildableBase::LoadBuildingSaveData(FBuildingSaveData LoadedData)
{
	MeshComponent->SetStaticMesh(LoadedData.BuildingMesh);
}

//ISavableInterface

//ISelectionInterface
void ABuildableBase::OnReceiveHover()
{
	for (int8 i = 0; i <= MeshComponent->GetMaterials().Num(); i++)
	{
		MeshComponent->SetMaterial(i, BuildingData->OnSelectionMaterial);
	}
}

void ABuildableBase::OnEndHover()
{
	for (int8 i = 0; i <= MeshComponent->GetMaterials().Num(); i++)
	{
		MeshComponent->SetMaterial(i, nullptr);
	}
}

void ABuildableBase::OnSelect()
{
	if (!IsSelected && BuildingData->SelectionWidget)
	{
		SelectionWidget = CreateWidget<UUI_SelectionBox>(GetWorld(), BuildingData->SelectionWidget);
		SelectionWidget->SetSelectedActor(this);
		SelectionWidget->AddToViewport(0);

		IsSelected = true;
	}
}

void ABuildableBase::OnEndSelect()
{
	if (IsSelected && SelectionWidget)
	{
		SelectionWidget->RemoveFromParent();
		IsSelected = false;
	}
}
//ISelectionInterface
