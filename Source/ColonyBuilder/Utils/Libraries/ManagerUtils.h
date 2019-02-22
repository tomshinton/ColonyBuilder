#pragma once

#include <Kismet/GameplayStatics.h>
#include "ColonyManager.h"
#include "ColonyInstance.h"

template<class M>
M* GetManager(UObject* Context)
{
	if (UColonyInstance* GameInst = Cast<UColonyInstance>(UGameplayStatics::GetGameInstance(Context)))
	{
		if (UColonyManager* FoundManager = GameInst->GetManager(M::StaticClass()))
		{
			return Cast<M>(FoundManager);
		}
	}
	return nullptr;
}




