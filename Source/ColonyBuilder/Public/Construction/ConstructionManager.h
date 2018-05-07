// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "Managers/ColonyManager.h"
#include "ConstructionManager.generated.h"

class UConstructionComponent;

UCLASS()
class COLONYBUILDER_API UConstructionManager : public UColonyManager
{
	GENERATED_BODY()

	static const float ConstructionTickRate;

public:

	void RegisterNewConstructionComponent(UConstructionComponent* NewComponent);

	bool IsComponentRegistered(UConstructionComponent* InComponent);

	virtual void PostInitProperties() override;
	
private:
	void TickComponents();

	TArray<UConstructionComponent*> RegisteredComponents;
	FTimerHandle TickComponentsHandle;
};
