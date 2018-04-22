// ColonyBuilder Project, personal project by Tom Shinton

#include "ColonySave.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY(SaveSlot);

UColonySave::UColonySave()
{
	SaveTime = FDateTime::Now();
}
