// ColonyBuilder Project, personal project by Tom Shinton

#include "ColonyInstance.h"

#include "SaveManager.h"


DEFINE_LOG_CATEGORY(ColonyInstanceLog);

void UColonyInstance::Init()
{
	UE_LOG(ColonyInstanceLog, Log, TEXT("Colony Instance started"));

	StartManager(USaveManager::StaticClass(), "Save Manager");
}

void UColonyInstance::StartManager(TSubclassOf<UColonyManager> ManagerClass, FString ManagerName)
{
	UClass* ClassToSpawn = ManagerClass;
	UColonyManager* NewManager = NewObject<UColonyManager>(this, ClassToSpawn);

	NewManager->SetManagerName(ManagerName);
	Managers.AddUnique(NewManager);
}

UColonyManager* UColonyInstance::GetManager(TSubclassOf<UColonyManager> ManagerClass)
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
