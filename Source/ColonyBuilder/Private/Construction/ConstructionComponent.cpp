// ColonyBuilder Project, personal project by Tom Shinton

#include "ConstructionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ColonyInstance.h"

#include "BuildingData.h"
#include "ConstructionManager.h"
#include "Utils/DataTypes/BuildingDataTypes.h"
#include "VillagerController.h"
#include "BaseVillager.h"
#include "Utils/Libraries/ManagerUtils.h"


DEFINE_LOG_CATEGORY(ConstructionComponentLog)

UConstructionComponent::UConstructionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	CurrStage = EConstructionStage::Construction;
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

				NewSite->OnNewBuilder.AddDynamic(this, &UConstructionComponent::NewLocalBuilder);
				NewSite->OnBuilderLeft.AddDynamic(this, &UConstructionComponent::LocalBuilderLeft);
			}
		}

		if (FoundConstructionSites.Num() == 0)
		{
			FString NoSitesWarning = GetOwner()->GetName() + " has no construction sites added";
			UE_LOG(ConstructionComponentLog, Error, TEXT("%s"), *NoSitesWarning);
		}
	}
}

FConstructionCallback UConstructionComponent::GetTickCallbackInfo()
{
	FConstructionCallback NewCallback(true, LocalBuilders.Num());
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

TWeakObjectPtr<UConstructionSiteComponent> UConstructionComponent::GetRandomConstructionSite()
{
	if (FoundConstructionSites.Num() > 0)
	{
		UConstructionSiteComponent* RandomConstructionSite = FoundConstructionSites[FMath::RandRange(0, FoundConstructionSites.Num() - 1)];
		return RandomConstructionSite;
	}
	
	return nullptr;
}

void UConstructionComponent::GetBuilders(TArray<AVillagerController*>& OutLocalBuilders, TArray<AVillagerController*>& OutRegisteredBuilders)
{
	for (const AVillagerController* VillagerController : LocalBuilders)
	{
		OutLocalBuilders.Add(const_cast<AVillagerController*>(VillagerController));
	}

	for (const AVillagerController* VillagerController : RegisteredBuilders)
	{
		OutRegisteredBuilders.Add(const_cast<AVillagerController*>(VillagerController));
	}
}

bool UConstructionComponent::RegisterNewConstruction()
{

	if (UConstructionManager* ConstructionManager = GetManager<UConstructionManager>(this))
	{
		ConstructionManager->RegisterNewConstructionComponent(this);

		if (ConstructionManager->IsComponentRegistered(this))
		{
			return true;
		}
	}

	return false;
}

bool UConstructionComponent::CanAcceptAnyMoreBuilders(AController* RequestingController)
{
	//Has this controller already been registered here? Ideally gate it off so we're not double assigning
	if (RegisteredBuilders.Contains(RequestingController))
	{
		return true;
	}

	if (RegisteredBuilders.Num() < BuildingData->MaxBuilders && (CurrStage == EConstructionStage::Construction || CurrStage == EConstructionStage::Upgrading))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool UConstructionComponent::CanFinish() const
{
	if (AActor* OwningActor = GetOwner())
	{
		TArray<AActor*> OverlappingActors;
		OwningActor->GetOverlappingActors(OverlappingActors, ABaseVillager::StaticClass());

		if (OverlappingActors.Num() > 0)
		{
			return false;
		}
		return true;
	}

	UE_LOG(ConstructionComponentLog, Error, TEXT("UConstructionComponent::CanFinish Component has no owning actor, returning false"));
	return false;
}

void UConstructionComponent::NewLocalBuilder(const AController* NewBuilder)
{
	if (const AVillagerController* NewController = Cast<AVillagerController>(NewBuilder))
	{
		if (RegisteredBuilders.Contains(NewController))
		{
			LocalBuilders.AddUnique(NewController);
		}
	}
}

void UConstructionComponent::LocalBuilderLeft(const AController* LeavingBuilder)
{
	if (const AVillagerController* LeavingController = Cast<AVillagerController>(LeavingBuilder))
	{
		LocalBuilders.Remove(LeavingController);
	}
}

void UConstructionComponent::RegisterNewBuilder(AVillagerController* RegisteredController)
{
	RegisteredBuilders.AddUnique(RegisteredController);
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
	BuildTimeLeft -= InUpdatedTime;

	float PercentageComplete = 1-(BuildTimeLeft / BuildingData->TotalBuildTime);

	OnConstructionUpdated.Broadcast(PercentageComplete);
}

void UConstructionComponent::FinishConstruction()
{
	CurrStage = EConstructionStage::Finished;
	OnConstructionFinished.Broadcast();
}

//IConstructionInterface
