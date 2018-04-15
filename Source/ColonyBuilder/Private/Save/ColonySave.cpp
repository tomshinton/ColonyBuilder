// ColonyBuilder Project, personal project by Tom Shinton

#include "ColonySave.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY(SaveSlot);

UColonySave::UColonySave()
{
	SlotName = "Dev Colony";
	SaveTime = FDateTime::Now();
}

void UColonySave::LogInfo()
{
	FString SlotNameCallback = "Slot Name: " + SlotName;
	FString SlotDateCallback = "Slot Date: " + SaveTime.ToString();

	UE_LOG(SaveSlot, Log, TEXT("- Save Successful -"));
	UE_LOG(SaveSlot, Log, TEXT("%s"), *SlotNameCallback);
	UE_LOG(SaveSlot, Log, TEXT("%s"), *SlotDateCallback);
}
