#pragma once

#include <Kismet/GameplayStatics.h>
#include "ColonyManager.h"
#include "ColonyInstance.h"

template<class M>
M* GetManager(UObject* Context)
{
	if (UColonyInstance* GameInst = Cast<UColonyInstance>(UGameplayStatics::GetGameInstance(Context)))
	{
		for (UColonyManager* Manager : GameInst->GetManagers())
		{
			if (Manager->IsA(M::StaticClass()))
			{
				M* ReturnManager = Cast<M>(Manager);
				return ReturnManager;
			}
		}
	}

	return nullptr;
}









