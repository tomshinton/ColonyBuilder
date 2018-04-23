// ColonyBuilder Project, personal project by Tom Shinton

#include "ColonySave.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/Engine.h"

DEFINE_LOG_CATEGORY(SaveSlot);

UColonySave::UColonySave()
{
	SaveTime = FDateTime::Now();
}
