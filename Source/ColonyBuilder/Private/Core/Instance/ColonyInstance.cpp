// ColonyBuilder Project, personal project by Tom Shinton

#include "ColonyInstance.h"

#include "SaveManager.h"
#include "Construction/ConstructionManager.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Managers/VillagerManager.h"
#include <Kismet/GameplayStatics.h>
#include <Algo/IntroSort.h>
#include "ColonyManagers.h"


DEFINE_LOG_CATEGORY(ColonyInstanceLog);

UColonyInstance::UColonyInstance()
{
	//Add classes here, with most critical manager later.  Pull from Editor
	if (UColonyManagers* ColonyManagers = GetMutableDefault<UColonyManagers>())
	{
		ManagerClasses = ColonyManagers->Managers;
	}
}

void UColonyInstance::Init()
{
	UE_LOG(ColonyInstanceLog, Log, TEXT("Colony Instance started"));

	UNavigationSystem* NavSys = UNavigationSystem::GetCurrent(GetWorld());
	if (NavSys)
	{
		NavSys->Build();
	}

	if (ManagerClasses.Num() > 0)
	{
		UE_LOG(ColonyInstanceLog, Log, TEXT("---> Spinning up managers <---"));
		Algo::Reverse(ManagerClasses);
		StartManager();
	}
}

UColonyManager* UColonyInstance::GetManager(TSubclassOf<UColonyManager> ManagerToFind)
{
	if (Managers.Contains(ManagerToFind))
	{
		return Managers[ManagerToFind];
	}

	return nullptr;
}

void UColonyInstance::StartManager()
{
	if (ManagerClasses.Num() <= 0)
	{
		UE_LOG(ColonyInstanceLog, Log, TEXT("---> Spinup Over <---"));
		return;
	}
	else
	{
		const TFunction<void()> Callback = [this]() { StartManager(); };

		const TSubclassOf<UColonyManager> ClassToSpawn = ManagerClasses.Pop(true);
		UColonyManager* NewManager = NewObject<UColonyManager>(this, ClassToSpawn);

		UE_LOG(ColonyInstanceLog, Log, TEXT("Spinning up %s"), *NewManager->GetManagerName());

		Managers.Add(ClassToSpawn, NewManager);
		NewManager->Init(Callback);
	}
}