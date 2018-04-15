// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ColonyManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(ColonyManagerLog, Log, All);

UCLASS(Abstract)
class COLONYBUILDER_API UColonyManager : public UObject
{
	GENERATED_BODY()	

public:
	void SetManagerName(FString InManagerName) { ManagerName = InManagerName; }
	
	virtual void PostInitProperties() override;

	virtual class UWorld* GetWorld() const override;

private:
	FString ManagerName;
};
