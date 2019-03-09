// ColonyBuilder Project, personal project by Tom Shinton

#include "ColonyManager.h"

DEFINE_LOG_CATEGORY(ColonyManagerLog);

void UColonyManager::Init(const TFunction<void()> InitCallback)
{
	UE_LOG(ColonyManagerLog, Log, TEXT("%s Started"), *GetName());
	InitCallback();
}

class UWorld* UColonyManager::GetWorld() const
{
	return GetOuter()->GetWorld();
}
