#pragma once

#include "CoreMinimal.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White ,text)
#define log(text) UE_LOG(LogTemp, Log, text);