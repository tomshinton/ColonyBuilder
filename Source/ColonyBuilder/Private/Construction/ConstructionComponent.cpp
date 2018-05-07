// ColonyBuilder Project, personal project by Tom Shinton

#include "ConstructionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ColonyInstance.h"

#include "BuildingData.h"
#include "ConstructionManager.h"

UConstructionComponent::UConstructionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UConstructionComponent::BeginPlay()
{
	if (GetOwner())
	{
		TArray<UConstructionSiteComponent*> FoundComponentsRaw;
		GetOwner()->GetComponents(FoundComponentsRaw);

		for (UActorComponent* FoundComponent : FoundComponentsRaw)
		{
			if (UConstructionSiteComponent* NewSite = Cast<UConstructionSiteComponent>(FoundComponent))
			{
				FoundConstructionSites.AddUnique(NewSite);
			}
		}
	}
}

FConstructionCallback UConstructionComponent::GetTickCallbackInfo()
{
	FConstructionCallback NewCallback(true, FoundConstructionSites.Num());

	//Get number of nearby assigned builders
	
	return NewCallback;
}

float UConstructionComponent::GetConstructionPercentageReadable()
{
	if (BuildingData)
	{
		return 1 - (BuildTimeLeft / BuildingData->TotalBuildTime);
	}
	
	return 0.f;
}

bool UConstructionComponent::RegisterNewConstruction()
{
	if (UColonyInstance* GameInst = Cast<UColonyInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		if (UConstructionManager* ConstructionManager = Cast<UConstructionManager>(GameInst->GetManagerByClass(UConstructionManager::StaticClass())))
		{
			ConstructionManager->RegisterNewConstructionComponent(this);

			if (ConstructionManager->IsComponentRegistered(this))
			{
				return true;
			}
		}
	}

	return false;
}

#pragma region SaveData
FConstructionSaveData UConstructionComponent::GetConstructionSaveData()
{
	float TotalBuildTime;

	if (BuildingData)
	{
		TotalBuildTime = BuildingData->TotalBuildTime;
	}
	else
	{
		TotalBuildTime = 0.f;
	}

	FConstructionSaveData NewData(CurrStage, BuildTimeLeft, TotalBuildTime);
	return NewData;
}

void UConstructionComponent::SetConstructionLoadData(FConstructionSaveData InLoadedData, UBuildingData* InBuildingData)
{
	CurrStage = InLoadedData.CurrentStage;
	BuildTimeLeft = InLoadedData.RemainingBuildTime;
	BuildingData = InBuildingData;

	if (CurrStage >= EConstructionStage::Finished)
	{
		FinishConstruction();
	}
	else
	{
		RegisterNewConstruction();
	}
}
#pragma endregion SaveData

//IConstructionInterface
void UConstructionComponent::StartConstruction(UBuildingData* InBuildingData)
{
	if (InBuildingData)
	{
		BuildingData = InBuildingData;
		BuildTimeLeft = BuildingData->TotalBuildTime;
	}

	RegisterNewConstruction();
}

void UConstructionComponent::UpdateConstructionTime(float InUpdatedTime)
{
	BuildTimeLeft -= (InUpdatedTime * FoundConstructionSites.Num());

	float PercentageComplete = 1-(BuildTimeLeft / BuildingData->TotalBuildTime);

	OnConstructionUpdated.Broadcast(PercentageComplete);
}

void UConstructionComponent::FinishConstruction()
{
	CurrStage = EConstructionStage::Finished;
	OnConstructionFinished.Broadcast();
}


//IConstructionInterface
