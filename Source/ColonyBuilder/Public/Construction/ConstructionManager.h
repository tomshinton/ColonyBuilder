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

	bool IsControllerOnActiveComponent(AController* InController) const;
	bool IsControllerOnPendingFinishComponent(AController* InController) const;

	TWeakObjectPtr<ABuildableBase> IsPawnRegisteredAsEmployee(ABaseVillager* InVillager) const;
	TWeakObjectPtr<ABuildableBase> IsPawnRegistedAsResident(ABaseVillager* InVillager) const;
	bool IsPawnGarrisoned(ABaseVillager* InVillager) const;
	void UngarrisonPawn(ABaseVillager* InVillager, const FGuid GarrisonBuilding);
	void GarrisonPawn(ABaseVillager* InVillager, const FGuid GarrisonBuilding);

	virtual void PostInitProperties() override;

	FGuid AssignPawnToWorkplace(ABaseVillager* InVillager);
	FGuid AssignPawnToResidence(ABaseVillager* InVillager);

	UConstructionComponent* ProcessNewConstructionRequest(AController* RequestingController, TWeakObjectPtr<UConstructionSiteComponent>& ConstructionSite);

	ABuildableBase* GetBuildingFromId(const FGuid BuildingId);

	UFUNCTION(BlueprintPure, Category = Construction)
	TArray<UConstructionComponent*> GetOnGoingConstructs() const { return RegisteredComponents; }
	
private:

	void TickComponents();

	UPROPERTY()
	TArray<UConstructionComponent*> RegisteredComponents;

	UPROPERTY()
	TArray<UConstructionComponent*> PendingFinishedComponents;

	UPROPERTY()
	TArray<ABuildableBase*> FinishedBuildings;

	FTimerHandle TickComponentsHandle;

	int32 CurrentRegisteredIndex;
};
