// ColonyBuilder Project, personal project by Tom Shinton
#include "ConstructionManager.h"

#include "../../Buildables/BuildableBase.h"
#include <EngineUtils.h>

DEFINE_LOG_CATEGORY(ConstructionManagerLog)

const float UConstructionManager::ConstructionTickRate(0.01);

UConstructionManager::UConstructionManager()
{
	ManagerName = "Construction Manager";
}

void UConstructionManager::Init(const TFunction<void() > InitCallback)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(TickComponentsHandle, this, &UConstructionManager::TickComponents, ConstructionTickRate, true, ConstructionTickRate);

		//Grab all preplaced-buildings and enable them as finished buildings
		for (TActorIterator<ABuildableBase> Itr(GetWorld()); Itr; ++Itr)
		{
			ABuildableBase* PreplacedBuilding = *Itr;

			if (PreplacedBuilding)
			{
				if (PreplacedBuilding->IsPreplaced)
				{
					PreplacedBuilding->ConstructionComponent->SetCurrConstructionStage(EConstructionStage::Finished);
					PreplacedBuilding->EnableBuilding();
					PreplacedBuilding->BuildingID = FGuid::NewGuid();
					FinishedBuildings.Add(PreplacedBuilding);
				}
			}
		}
	}

	Super::Init(InitCallback);
}

FGuid UConstructionManager::AssignPawnToWorkplace(ABaseVillager* InVillager)
{
	//Current implementation assigns pawn to first available vacancy
	for (TWeakObjectPtr<ABuildableBase> FinishedBuilding : FinishedBuildings)
	{
		if (ABuildableBase* BuildingPtr = FinishedBuilding.Get())
		{
			if (BuildingPtr->HasVacancies())
			{
				BuildingPtr->AddEmployee(InVillager);
				return BuildingPtr->BuildingID;
			}
		}
	}

	return FGuid(0, 0, 0, 0);
}

FGuid UConstructionManager::AssignPawnToResidence(ABaseVillager* InVillager)
{
	for (TWeakObjectPtr<ABuildableBase> FinishedBuilding : FinishedBuildings)
	{
		if (ABuildableBase* BuildingPtr = FinishedBuilding.Get())
		{
			if (BuildingPtr->HasBoardingRoom())
			{
				BuildingPtr->AddResident(InVillager);
				return BuildingPtr->BuildingID;
			}
		}
	}

	return FGuid(0, 0, 0, 0);
}

void UConstructionManager::TickComponents()
{
	if (RegisteredComponents.Num() > 0)
	{
		if (UConstructionComponent* CurrComponent = RegisteredComponents[CurrentRegisteredIndex])
		{
			if (CurrComponent->GetTickCallbackInfo().CanTick)
			{
				const float UpdateAmount = CurrComponent->GetTickCallbackInfo().TickModifier*ConstructionTickRate * (RegisteredComponents.Num() + 1);
				CurrComponent->UpdateConstructionTime(UpdateAmount);

				if (CurrComponent->GetCurrentBuildTime() <= 0.f)
				{
					RegisteredComponents.Remove(CurrComponent);
					PendingFinishedComponents.Add(CurrComponent);
				}
			}
		}
	}

	(CurrentRegisteredIndex + 1) > RegisteredComponents.Num() - 1 ? CurrentRegisteredIndex = 0 : CurrentRegisteredIndex++;

	if (PendingFinishedComponents.Num() > 0)
	{
		for (int32 i = PendingFinishedComponents.Num() - 1; i >= 0; i--)
		{
			if (UConstructionComponent* ComponentPtr = PendingFinishedComponents[i])
			{
				if (ComponentPtr->CanFinish())
				{
					ComponentPtr->FinishConstruction();
					PendingFinishedComponents.RemoveAt(i);

					if (ABuildableBase* FinishedBuilding = Cast<ABuildableBase>(ComponentPtr->GetOwner()))
					{
						FinishedBuildings.Add(FinishedBuilding);
					}
				}
			}
		}
	}
}

UConstructionComponent* UConstructionManager::ProcessNewConstructionRequest(AController* RequestingController, TWeakObjectPtr<UConstructionSiteComponent>& ConstructionSite)
{
	//for (TWeakObjectPtr<UConstructionComponent> RegisteredComponent : RegisteredComponents)
	//{
	//	if (RegisteredComponent->CanAcceptAnyMoreBuilders(RequestingController))
	//	{
	//		if (AVillagerController* VillagerController = Cast<AVillagerController>(RequestingController))
	//		{
	//			//RegisteredComponent->RegisterNewBuilder(VillagerController);
	//			ConstructionSite = RegisteredComponent->GetRandomConstructionSite();
	//			return RegisteredComponent.Get();
	//		}
	//	}
	//}
	return nullptr;
}

ABuildableBase* UConstructionManager::GetBuildingFromId(const FGuid BuildingId)
{
	return *FinishedBuildings.FindByPredicate([&](ABuildableBase* Building) { return Building->BuildingID == BuildingId; });
}

void UConstructionManager::RegisterNewConstructionComponent(UConstructionComponent* NewComponent)
{
	RegisteredComponents.AddUnique(NewComponent);
}

bool UConstructionManager::IsComponentRegistered(UConstructionComponent* InComponent)
{
	if (RegisteredComponents.Contains(InComponent))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool UConstructionManager::IsControllerOnActiveComponent(AController* InController) const
{
	bool IsRegistered = false;

	for (TWeakObjectPtr<UConstructionComponent> RegisteredComponent : RegisteredComponents)
	{
//		if(RegisteredComponent.Get()->GetRegisteredBuilders().Contains(InController))
		//{
		//	return true;
		//}
	}

	return false;
}

bool UConstructionManager::IsControllerOnPendingFinishComponent(AController* InController) const
{
	bool IsRegistered = false;
	/*
		for (TWeakObjectPtr<UConstructionComponent> PendingComponent : PendingFinishedComponents)
		{
			if (PendingComponent.Get()->GetRegisteredBuilders().Contains(InController))
			{
				return true;
			}
		}
	*/
	return false;
}

TWeakObjectPtr<ABuildableBase> UConstructionManager::IsPawnRegisteredAsEmployee(ABaseVillager* InVillager) const
{
	//Is this pawn registered to any of of our Finished Buildings as an employee?

	for (TWeakObjectPtr<ABuildableBase> FinishedBuilding : FinishedBuildings)
	{
		if (FinishedBuilding.IsValid())
		{
			if (FinishedBuilding.Get()->RegisteredEmployees.Contains(InVillager->VillagerID))
			{
				return FinishedBuilding;
			}
		}
	}

	return nullptr;
}

TWeakObjectPtr<ABuildableBase> UConstructionManager::IsPawnRegistedAsResident(ABaseVillager* InVillager) const
{
	//Is this pawn registered to any of our Finished Buildings as a Resident? 

	for (TWeakObjectPtr<ABuildableBase> FinishedBuilding : FinishedBuildings)
	{
		if (FinishedBuilding.IsValid())
		{
			if (FinishedBuilding.Get()->RegisteredResidents.Contains(InVillager->VillagerID))
			{
				return FinishedBuilding;
			}
		}
	}

	return nullptr;
}

bool UConstructionManager::IsPawnGarrisoned(ABaseVillager* InVillager) const
{
	for (ABuildableBase* FinishedBuilding : FinishedBuildings)
	{
		TArray<UActorComponent*> ActorComps = FinishedBuilding->GetComponentsByClass(UGarrisonPoint::StaticClass());
		for (UActorComponent* ActorComp : ActorComps)
		{
			if (UGarrisonPoint* GarrisonPoint = Cast<UGarrisonPoint>(ActorComp))
			{
				if (GarrisonPoint->IsPawnGarrisoned(InVillager))
				{
					return true;
				}
			}
		}
	}

	return false;
}

void UConstructionManager::UngarrisonPawn(ABaseVillager* InVillager, const FGuid GarrisonBuilding)
{
	if (ABuildableBase* FoundBuilding = GetBuildingFromId(GarrisonBuilding))
	{
		FoundBuilding->GetGarrisonPoint()->Ungarrison(InVillager);
	}
}

void UConstructionManager::GarrisonPawn(ABaseVillager* InVillager, const FGuid GarrisonBuilding)
{
	if (ABuildableBase* FoundBuilding = GetBuildingFromId(GarrisonBuilding))
	{
		FoundBuilding->GetGarrisonPoint()->Garrison(InVillager);
	}
}

