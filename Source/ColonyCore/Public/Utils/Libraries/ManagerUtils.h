#pragma once

#include <Kismet/GameplayStatics.h>
class UColonyManager;
class UColonyInstance;

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




