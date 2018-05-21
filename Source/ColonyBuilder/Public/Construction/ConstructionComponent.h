// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ConstructionInterface.h"
#include "Utils/DataTypes/SaveDataTypes.h"
#include "ConstructionSiteComponent.h"
#include "VillagerController.h"

#include "ConstructionComponent.generated.h"

class UBuildingData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConstructionFinished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConstructionUpdated, const float&, PercentageComplete);

DECLARE_LOG_CATEGORY_EXTERN(ConstructionComponentLog, All, All)

USTRUCT(BlueprintType)
struct FConstructionCallback
{
	GENERATED_USTRUCT_BODY()

	FConstructionCallback() :
	CanTick(false)
	, TickModifier(1)
	{}

	FConstructionCallback(bool InCanTick, uint32 InTickModifier) :
	CanTick(InCanTick)
	, TickModifier(InTickModifier)
	{}

	bool CanTick;
	uint32 TickModifier;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COLONYBUILDER_API UConstructionComponent : public UActorComponent, public IConstructionInterface
{
	GENERATED_BODY()

public:	

	UConstructionComponent();
		
public:

 	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category = Construction)
	float GetConstructionPercentageReadable();

	UFUNCTION(BlueprintPure, Category = Construction)
	FVector GetConstructionSiteLocation();

	UFUNCTION(BlueprintPure, Category = Construction)
	EConstructionStage GetCurrConstructionStage() { return CurrStage; }

	UFUNCTION(BlueprintCallable, Category = Construction)
	void GetBuilders(TArray<AVillagerController*>& OutLocalBuilders, TArray<AVillagerController*>& OutRegisteredBuilders);

	TArray<const AVillagerController*> const GetRegisteredBuilders() { return RegisteredBuilders; }
	TArray<const AVillagerController*> const GetLocalBuilders() { return LocalBuilders; }

	UPROPERTY(BlueprintAssignable, Category = Construction)
	FOnConstructionUpdated OnConstructionUpdated;
	UPROPERTY(BlueprintAssignable, Category = Construction)
	FOnConstructionFinished OnConstructionFinished;

	FConstructionCallback GetTickCallbackInfo();
	float GetCurrentBuildTime() { return BuildTimeLeft; }

	FConstructionSaveData GetConstructionSaveData();
	void SetConstructionLoadData(FConstructionSaveData InLoadedData, UBuildingData* InBuildingData);

	bool CanAcceptAnyMoreBuilders(AController* RequestingController);

	UFUNCTION()
	void NewLocalBuilder(const AController* NewBuilder);
	UFUNCTION()
	void LocalBuilderLeft(const AController* LeavingBuilder);

	void RegisterNewBuilder(AVillagerController* RegisteredController);


	//IConstructionInterface
	virtual void StartConstruction(UBuildingData* InBuildingData) override;
	virtual void UpdateConstructionTime(float InUpdatedTime) override;
	virtual void FinishConstruction() override;
	//IConstructionInterface


private:
	bool RegisterNewConstruction();

	UBuildingData* BuildingData;
	float BuildTimeLeft;
	TArray<UConstructionSiteComponent*> FoundConstructionSites;

	EConstructionStage CurrStage;

	FConstructionSaveData SaveData;

	TArray<const AVillagerController*> RegisteredBuilders;
	TArray<const AVillagerController*> LocalBuilders;
};
