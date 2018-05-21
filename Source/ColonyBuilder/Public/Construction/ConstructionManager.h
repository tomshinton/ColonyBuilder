// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "Managers/ColonyManager.h"
#include "ConstructionManager.generated.h"

class UConstructionComponent;
class AVillagerController;

DECLARE_LOG_CATEGORY_EXTERN(ConstructionManagerLog, All, All)

UCLASS(Blueprintable)
class COLONYBUILDER_API UConstructionManager : public UColonyManager
{
	GENERATED_BODY()

	static const float ConstructionTickRate;

public:

	void RegisterNewConstructionComponent(UConstructionComponent* NewComponent);
	bool IsComponentRegistered(UConstructionComponent* InComponent);

	bool IsControllerAlreadyRegistered(AVillagerController* InController);
	virtual void PostInitProperties() override;

	UFUNCTION(BlueprintCallable, Category = Construction)
	UConstructionComponent* ProcessNewConstructionRequest(AController* RequestingController, FVector& SiteLocation);

	UFUNCTION(BlueprintPure, Category = Construction)
	TArray<UConstructionComponent*> GetOnGoingConstructs() { return RegisteredComponents; }
	
private:
	void TickComponents();

	TArray<UConstructionComponent*> RegisteredComponents;
	FTimerHandle TickComponentsHandle;
};
