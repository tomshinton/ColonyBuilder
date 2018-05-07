// ColonyBuilder Project, personal project by Tom Shinton

#include "ConstructionManager.h"
#include "Construction/ConstructionComponent.h"

const float UConstructionManager::ConstructionTickRate(0.1);

void UConstructionManager::TickComponents()
{
	for (int32 i = RegisteredComponents.Num()-1; i>=0; i--)
	{
		UConstructionComponent* CurrComponent = RegisteredComponents[i];

		if (CurrComponent->GetTickCallbackInfo().CanTick)
		{
			CurrComponent->UpdateConstructionTime(ConstructionTickRate);

			if (CurrComponent->GetCurrentBuildTime() <= 0.f)
			{
				CurrComponent->FinishConstruction();
				RegisteredComponents.Remove(CurrComponent);
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
