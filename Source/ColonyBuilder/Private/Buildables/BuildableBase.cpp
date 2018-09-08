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
	MeshComponent->SetCollisionProfileName("ConstructionSite");
	MeshComponent->SetupAttachment(SceneRoot);
	MeshComponent->bGenerateOverlapEvents = true;

	ConstructionComponent = CreateDefaultSubobject<UConstructionComponent>(TEXT("Construction Component"));

	PrimaryActorTick.bCanEverTick = true;
}

void ABuildableBase::OnConstruction(const FTransform& Transform)
{
	if (BuildingData)
	{
		MeshComponent->SetStaticMesh(BuildingData->BuildingBaseMesh);
#if WITH_EDITOR
		SetFolderPath(FName(*BuildingData->GetFullCategoryAsString()));
#endif //WITH_EDITOR

		SelectionWidget = CreateWidget<UUI_SelectionBox>(GetWorld(), BuildingData->SelectionWidget);
		SelectionWidget->SetSelectedActor(this);

		ConstructionComponent->OnConstructionUpdated.AddDynamic(SelectionWidget.Get(), &UUI_SelectionBox::OnConstructionUpdated);
	}
}

#pragma region SavableInterface

void ABuildableBase::EnableBuilding()
{
	if (SelectionWidget.IsValid())
	{
		SelectionWidget.Get()->OnConstructionFinished();
	}

	MeshComponent->SetCollisionProfileName("Building");
}

FBuildingSaveData ABuildableBase::GetBuildingSaveData()
{
	FBuildingSaveData NewData(BuildingID, GetClass(), BuildingData, GetActorTransform(), MeshComponent->GetStaticMesh(), ConstructionComponent->GetConstructionSaveData(), RegisteredEmployees, RegisteredResidents);
	return NewData;
}

void ABuildableBase::LoadBuildingSaveData(FBuildingSaveData LoadedData)
{
	MeshComponent->SetStaticMesh(LoadedData.BuildingMesh);
	ConstructionComponent->SetConstructionLoadData(LoadedData.ConstructionData, BuildingData);

	if (ConstructionComponent->GetCurrConstructionStage() == EConstructionStage::Finished)
	{
		EnableBuilding();
	}

	BuildingID = LoadedData.ID;
}

#pragma endregion SavableInterface

#pragma region SelectionInterface
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
	if (!IsSelected && SelectionWidget.IsValid())
	{
		SelectionWidget.Get()->AddToViewport(0);
		IsSelected = true;
	}
}

void ABuildableBase::OnEndSelect()
{
	if (IsSelected && SelectionWidget.IsValid())
	{
		SelectionWidget.Get()->RemoveFromParent();
		IsSelected = false;
	}
}
#pragma endregion SelectionInterface
