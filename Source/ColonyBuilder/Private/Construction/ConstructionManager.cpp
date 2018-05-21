// ColonyBuilder Project, personal project by Tom Shinton

#include "ConstructionManager.h"
#include "Construction/ConstructionComponent.h"

DEFINE_LOG_CATEGORY(ConstructionManagerLog)

const float UConstructionManager::ConstructionTickRate(0.1);

void UConstructionManager::TickComponents()
{
	if (RegisteredComponents.Num() > 0)
	{
		for (int32 i = RegisteredComponents.Num() - 1; i >= 0; i--)
		{
			UConstructionComponent* CurrComponent = RegisteredComponents[i];

			if (CurrComponent->GetTickCallbackInfo().CanTick)
			{
				CurrComponent->UpdateConstructionTime(CurrComponent->GetTickCallbackInfo().TickModifier*ConstructionTickRate);

				if (CurrComponent->GetCurrentBuildTime() <= 0.f)
				{
					CurrComponent->FinishConstruction();
					RegisteredComponents.Remove(CurrComponent);
				}
			}
		}
	}
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

void UConstructionManager::PostInitProperties()
{
	Super::PostInitProperties();

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(TickComponentsHandle, this, &UConstructionManager::TickComponents, ConstructionTickRate, true, ConstructionTickRate);
	}
}

UConstructionComponent* UConstructionManager::ProcessNewConstructionRequest(AController* RequestingController, FVector& SiteLocation)
{
	for (UConstructionComponent* RegisteredComponent : RegisteredComponents)
	{
		if (RegisteredComponent->CanAcceptAnyMoreBuilders(RequestingController))
		{
			if (AVillagerController* VillagerController = Cast<AVillagerController>(RequestingController))
			{
				RegisteredComponent->RegisterNewBuilder(VillagerController);
				SiteLocation = RegisteredComponent->GetConstructionSiteLocation();
				return RegisteredComponent;
			}
		}
	}
	return nullptr;
}
