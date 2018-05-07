// ColonyBuilder Project, personal project by Tom Shinton

#include "ColonyInstance.h"

#include "SaveManager.h"
#include "Construction/ConstructionManager.h"


DEFINE_LOG_CATEGORY(ColonyInstanceLog);

void UColonyInstance::Init()
{
	UE_LOG(ColonyInstanceLog, Log, TEXT("Colony Instance started"));

	//Gameplay critical managers - need to be instantiated first.
	StartManager(UConstructionManager::StaticClass(), "Construction Manager");

	StartManager(USaveManager::StaticClass(), "Save Manager");
}

void UColonyInstance::StartManager(TSubclassOf<UColonyManager> ManagerClass, FString ManagerName)
{
	UClass* ClassToSpawn = ManagerClass;
	UColonyManager* NewManager = NewObject<UColonyManager>(this, ClassToSpawn);

	NewManager->SetManagerName(ManagerName);
	Managers.AddUnique(NewManager);
}

UColonyManager* UColonyInstance::GetManagerByClass(TSubclassOf<UColonyManager> ManagerClass)
{
	for (UColonyManager* Manager : Managers)
	{
		if (Manager->IsA(ManagerClass))
		{
			return Manager;
		}

	}
	return nullptr;
}
