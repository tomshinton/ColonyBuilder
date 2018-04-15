// ColonyBuilder Project, personal project by Tom Shinton

#include "ColonyManager.h"

DEFINE_LOG_CATEGORY(ColonyManagerLog);

void UColonyManager::PostInitProperties()
{
	Super::PostInitProperties();

	FString ManagerStartedText = GetName() + " started";
	UE_LOG(ColonyManagerLog, Log, TEXT("%s"), *ManagerStartedText);
}

class UWorld* UColonyManager::GetWorld() const
{
	return GetOuter()->GetWorld();
}
