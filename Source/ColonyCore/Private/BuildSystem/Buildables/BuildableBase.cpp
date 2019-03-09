// ColonyBuilder Project, personal project by Tom Shinton

#include "BuildableBase.h"
#include "BuildingData.h"
#include "UI/Selection/UI_SelectionBox.h"
#include "../Construction/ConstructionComponent.h"

ABuildableBase::ABuildableBase()
	: SceneRoot(CreateDefaultSubobject<USceneComponent>(TEXT("Building Root")))
	, MeshComponent(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Building Static Mesh")))
	, ConstructionComponent(CreateDefaultSubobject<UConstructionComponent>(TEXT("Construction Component")))
{
	RootComponent = SceneRoot;

	MeshComponent->SetCollisionProfileName("ConstructionSite");
	MeshComponent->SetupAttachment(SceneRoot);
	MeshComponent->bGenerateOverlapEvents = true;

	PrimaryActorTick.bCanEverTick = false;
}

void ABuildableBase::OnConstruction(const FTransform& Transform)
{
	if (UWorld* World = GetWorld())
	{
		if (BuildingData)
		{
			MeshComponent->SetStaticMesh(BuildingData->BuildingBaseMesh);

#if WITH_EDITOR
			SetFolderPath(FName(*BuildingData->GetFullCategoryAsString()));
#endif //WITH_EDITOR

			if (BuildingData->SelectionWidget)
			{
				SelectionWidget = CreateWidget<UUI_SelectionBox>(World, BuildingData->SelectionWidget);
				SelectionWidget->SetSelectedActor(this);

				ConstructionComponent->OnConstructionUpdated.AddDynamic(SelectionWidget, &UUI_SelectionBox::OnConstructionUpdated);
			}
		}
	}

	CachedGarrisonPoint = Cast<UGarrisonPoint>(GetComponentByClass(UGarrisonPoint::StaticClass()));
}

void ABuildableBase::EnableBuilding()
{
	if (SelectionWidget)
	{
		SelectionWidget->OnConstructionFinished();
	}

	MeshComponent->SetCollisionProfileName("Building");
}

void ABuildableBase::AddEmployee(ABaseVillager* InVillager)
{
	RegisteredEmployees.AddUnique(InVillager->VillagerID);

	InVillager->WorkplaceID = BuildingID;

	if (BuildingData->Professions.Num() > 0)
	{
		InVillager->SetProfession(BuildingData->Professions[0]);
	}
}

void ABuildableBase::AddResident(ABaseVillager* InVillager)
{
	RegisteredResidents.AddUnique(InVillager->VillagerID);
	InVillager->ResidenceID = BuildingID;
}

bool ABuildableBase::HasVacancies() const
{
	if (RegisteredEmployees.Num() < BuildingData->MaxEmployees)
	{
		return true;
	} 
	else
	{
		return false;
	}
}

bool ABuildableBase::HasBoardingRoom() const
{
	if (RegisteredResidents.Num() < BuildingData->MaxResidents)
	{
		return true;
	}
	else
	{
		return false;
	}
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
	if (!IsSelected && SelectionWidget)
	{
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

