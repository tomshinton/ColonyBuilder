// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "Managers/ColonyManager.h"
#include "ConstructionSiteComponent.h"
#include "BaseVillager.h"
#include "ConstructionManager.generated.h"

class UConstructionComponent;
class AVillagerController;
class ABuildableBase;

DECLARE_LOG_CATEGORY_EXTERN(ConstructionManagerLog, All, All)

//////////////////////////////////////////////////////////////////////////
// Construction Manager - UObject in charge or managing all ongoing construction projects
//////////////////////////////////////////////////////////////////////////

UCLASS(Blueprintable)
class COLONYBUILDER_API UConstructionManager : public UColonyManager
{
	GENERATED_BODY()

	static const float ConstructionTickRate;

public:

	void RegisterNewConstructionComponent(UConstructionComponent* NewComponent);
	bool IsComponentRegistered(UConstructionComponent* InComponent);

	bool IsControllerOnActiveComponent(AVillagerController* InController) const;
	bool IsControllerOnPendingFinishComponent(AVillagerController* InController) const;

	TWeakObjectPtr<ABuildableBase> IsPawnRegisteredAsEmployee(ABaseVillager* InVillager) const;
	TWeakObjectPtr<ABuildableBase> IsPawnRegistedAsResident(ABaseVillager* InVillager) const;

	virtual void PostInitProperties() override;

	UConstructionComponent* ProcessNewConstructionRequest(AController* RequestingController, TWeakObjectPtr<UConstructionSiteComponent>& ConstructionSite);

	UFUNCTION(BlueprintPure, Category = Construction)
	TArray<UConstructionComponent*> GetOnGoingConstructs() { return GetRegisteredComponentsAsRaw(); }
	
	TArray<UConstructionComponent*> GetRegisteredComponentsAsRaw()
	{
		TArray<UConstructionComponent*> Constructs;

		for (TWeakObjectPtr<UConstructionComponent> Component : RegisteredComponents)
		{
			if (Component.IsValid())
			{
				Constructs.Add(Component.Get());
			}
		}

		return Constructs;
	}

private:

	void TickComponents();

	UPROPERTY()
	TArray<TWeakObjectPtr<UConstructionComponent>> RegisteredComponents;

	UPROPERTY()
	TArray<TWeakObjectPtr<UConstructionComponent>> PendingFinishedComponents;

	UPROPERTY()
	TArray<TWeakObjectPtr<ABuildableBase>> FinishedBuildings;

	FTimerHandle TickComponentsHandle;

	int32 CurrentRegisteredIndex;
};
