// ColonyBuilder Project, personal project by Tom Shinton

#include "ColonySave.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/Engine.h"

DEFINE_LOG_CATEGORY(SaveSlot);

UColonySave::UColonySave()
{
	SaveTime = FDateTime::Now();
}

void UColonySave::Heartbeat()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 1, FColor::White, TEXT("SlotName: %s"), *SlotName);
		GEngine->AddOnScreenDebugMessage(2, 1, FColor::White, TEXT("SavedActors: %s"), *FString::FromInt(SavedBuildables.Num()));
		GEngine->AddOnScreenDebugMessage(3, 1, FColor::White, TEXT("SaveTime: %s"), *SaveTime.ToString());
	}
}
