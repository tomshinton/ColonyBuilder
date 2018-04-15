#pragma once

#include "CoreMinimal.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White ,text)
#define printKey(key, text) if (GEngine) GEngine->AddOnScreenDebugMessage(key, 1.5, FColor::Emerald ,text)

#define log(text) UE_LOG(LogTemp, Log, text);