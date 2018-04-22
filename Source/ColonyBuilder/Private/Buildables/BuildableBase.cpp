// ColonyBuilder Project, personal project by Tom Shinton

#include "BuildableBase.h"
#include "BuildingData.h"


// Sets default values
ABuildableBase::ABuildableBase()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Building Root"));
	RootComponent = SceneRoot;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Building Static Mesh"));
	MeshComponent->SetCollisionProfileName("OverlapAllDynamic");
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

FSaveData ABuildableBase::GetSaveData()
{
	FBuildingSaveData SaveData;
	
	SaveData.BuildingClass = GetClass();
	SaveData.BuildingTransform = GetActorTransform();

	
	FString SavableCallback = GetName() + " interacting with SaveSystem";
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Emerald, SavableCallback);

	return SaveData;
}
