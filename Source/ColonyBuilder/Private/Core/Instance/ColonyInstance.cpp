// ColonyBuilder Project, personal project by Tom Shinton

#include "ColonyInstance.h"

#include "SaveManager.h"
#include "Construction/ConstructionManager.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Managers/VillagerManager.h"


DEFINE_LOG_CATEGORY(ColonyInstanceLog);

void UColonyInstance::Init()
{
	UE_LOG(ColonyInstanceLog, Log, TEXT("Colony Instance started"));

	UNavigationSystem* NavSys = UNavigationSystem::GetCurrent(GetWorld());
	if (NavSys)
	{
		NavSys->Build();
	}

	//Gameplay critical managers - need to be instantiated first.
	StartManager(UConstructionManager::StaticClass(), "Construction Manager");
	StartManager(UVillagerManager::StaticClass(), "Villager Manager");
	StartManager(USaveManager::StaticClass(), "Save Manager");
}

void UColonyInstance::StartManager(TSubclassOf<UColonyManager> ManagerClass, FString ManagerName)
{
	UClass* ClassToSpawn = ManagerClass;
	UColonyManager* NewManager = NewObject<UColonyManager>(this, ClassToSpawn);

	NewManager->SetManagerName(ManagerName);
	Managers.AddUnique(NewManager);
}

UColonyManager* UColonyInstance::GetManagerFromClass(TSubclassOf<UColonyManager> InManagerClass)
{
	for (UColonyManager* Manager : Managers)
	{
		if (Manager->IsA(InManagerClass))
		{
			return Manager;
		}
	}
	return nullptr;
}
