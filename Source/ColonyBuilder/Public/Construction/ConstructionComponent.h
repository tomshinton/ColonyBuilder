// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ConstructionInterface.h"
#include "Utils/DataTypes/SaveDataTypes.h"

#include "ConstructionComponent.generated.h"

class UBuildingData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConstructionFinished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConstructionUpdated, const float&, PercentageComplete);

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

	FConstructionCallback GetTickCallbackInfo();
	float GetCurrentBuildTime() { return BuildTimeLeft; }

	UFUNCTION(BlueprintPure, Category = Construction)
	float GetConstructionPercentageReadable();

	UPROPERTY(BlueprintAssignable, Category = Construction)
	FOnConstructionUpdated OnConstructionUpdated;
	UPROPERTY(BlueprintAssignable, Category = Construction)
	FOnConstructionFinished OnConstructionFinished;

	FConstructionSaveData GetConstructionSaveData();
	void SetConstructionLoadData(FConstructionSaveData InLoadedData, UBuildingData* InBuildingData);

	//IConstructionInterface
	virtual void StartConstruction(UBuildingData* InBuildingData) override;
	virtual void UpdateConstructionTime(float InUpdatedTime) override;
	virtual void FinishConstruction() override;
	//IConstructionInterface

private:
	bool RegisterNewConstruction();

	UBuildingData* BuildingData;
	float BuildTimeLeft;

	EConstructionStage CurrStage;

	FConstructionSaveData SaveData;
};
