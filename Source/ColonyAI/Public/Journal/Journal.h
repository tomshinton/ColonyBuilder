// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "Keys.h"

#include "Journal.generated.h"

//////////////////////////////////////////////////////////////////////////
// Base class for all Important info that should be passed around plans/stages.
// Includes temporary information put there by other stages for use elsewhere, eg MoveToLocation used by the MoveTo Stage
//////////////////////////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FJournal
{
	GENERATED_USTRUCT_BODY()

	FJournal(){};

public:

	TArray<FJournalKey> Keys;
};