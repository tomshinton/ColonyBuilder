// ColonyBuilder Project, personal project by Tom Shinton

#include "ConstructionManager.h"
#include "Construction/ConstructionComponent.h"
#include "BuildableBase.h"

DEFINE_LOG_CATEGORY(ConstructionManagerLog)

const float UConstructionManager::ConstructionTickRate(0.01);

void UConstructionManager::PostInitProperties()
{
	Super::PostInitProperties();

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(TickComponentsHandle, this, &UConstructionManager::TickComponents, ConstructionTickRate, true, ConstructionTickRate);
	}
}

void UConstructionManager::TickComponents()
{
	if (RegisteredComponents.Num() > 0)
	{
		if (UConstructionComponent* CurrComponent = RegisteredComponents[CurrentRegisteredIndex].Get())
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
			if (UConstructionComponent* ComponentPtr = PendingFinishedComponents[i].Get())
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
	for (TWeakObjectPtr<UConstructionComponent> RegisteredComponent : RegisteredComponents)
	{
		if (RegisteredComponent->CanAcceptAnyMoreBuilders(RequestingController))
		{
			if (AVillagerController* VillagerController = Cast<AVillagerController>(RequestingController))
			{
				RegisteredComponent->RegisterNewBuilder(VillagerController);
				ConstructionSite = RegisteredComponent->GetRandomConstructionSite();
				return RegisteredComponent.Get();
			}
		}
	}
	return nullptr;
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

bool UConstructionManager::IsControllerOnActiveComponent(AVillagerController* InController) const
{
	bool IsRegistered = false;

	for (TWeakObjectPtr<UConstructionComponent> RegisteredComponent : RegisteredComponents)
	{
		if(RegisteredComponent.Get()->GetRegisteredBuilders().Contains(InController))
		{
			return true;
		}
	}

	return false;
}

bool UConstructionManager::IsControllerOnPendingFinishComponent(AVillagerController* InController) const
{
	bool IsRegistered = false;

	for (TWeakObjectPtr<UConstructionComponent> PendingComponent : PendingFinishedComponents)
	{
		if (PendingComponent.Get()->GetRegisteredBuilders().Contains(InController))
		{
			return true;
		}
	}

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

